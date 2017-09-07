#include <linux/skbuff.h>
#include <linux/export.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/if_vlan.h>
#include <net/ip.h>
#include <linux/if_tunnel.h>
#include <linux/if_pppox.h>
#include <linux/ppp_defs.h>
#include <net/flow_keys.h>
#include <linux/jump_label.h>

/* copy saddr & daddr, possibly using 64bit load/store
 * Equivalent to :	flow->src = iph->saddr;
 *			flow->dst = iph->daddr;
 */
static void iph_to_flow_copy_addrs(struct flow_keys *flow, const struct iphdr *iph)
{
	BUILD_BUG_ON(offsetof(typeof(*flow), dst) !=
		     offsetof(typeof(*flow), src) + sizeof(flow->src));
	memcpy(&flow->src, &iph->saddr, sizeof(flow->src) + sizeof(flow->dst));
}

/**
 * __skb_flow_get_ports - extract the upper layer ports and return them
 * @skb: sk_buff to extract the ports from
 * @thoff: transport header offset
 * @ip_proto: protocol for which to get port offset
 * @data: raw buffer pointer to the packet, if NULL use skb->data
 * @hlen: packet header length, if @data is NULL use skb_headlen(skb)
 *
 * The function will try to retrieve the ports at offset thoff + poff where poff
 * is the protocol port offset returned from proto_ports_offset
 */
__be32 __skb_flow_get_ports(const struct sk_buff *skb, int thoff, u8 ip_proto,
			    void *data, int hlen)
{
	int poff = proto_ports_offset(ip_proto);

	if (!data) {
		data = skb->data;
		hlen = skb_headlen(skb);
	}

	if (poff >= 0) {
		__be32 *ports, _ports;

		ports = __skb_header_pointer(skb, thoff + poff,
					     sizeof(_ports), data, hlen, &_ports);
	/* 可能会有问题*/
	//	if (ports)
	//		return (__be32)net_hdr_word(ports);
	}

	return 0;
}
EXPORT_SYMBOL(__skb_flow_get_ports);


bool skb_flow_dissect(const struct sk_buff *skb, struct flow_keys *flow)
{
	int poff, nhoff = skb_network_offset(skb);
	u8 ip_proto;
	__be16 proto = skb->protocol;

	memset(flow, 0, sizeof(*flow));

again:
	switch (proto) {
	case __constant_htons(ETH_P_IP): {
		const struct iphdr *iph;
		struct iphdr _iph;
ip:
		iph = skb_header_pointer(skb, nhoff, sizeof(_iph), &_iph);
		if (!iph)
			return false;

		if (ip_is_fragment(iph))
			ip_proto = 0;
		else
			ip_proto = iph->protocol;
		iph_to_flow_copy_addrs(flow, iph);
		nhoff += iph->ihl * 4;
		break;
	}
	case __constant_htons(ETH_P_IPV6): {
		const struct ipv6hdr *iph;
		struct ipv6hdr _iph;
ipv6:
		iph = skb_header_pointer(skb, nhoff, sizeof(_iph), &_iph);
		if (!iph)
			return false;

		ip_proto = iph->nexthdr;
		flow->src = iph->saddr.s6_addr32[3];
		flow->dst = iph->daddr.s6_addr32[3];
		nhoff += sizeof(struct ipv6hdr);
		break;
	}
	case __constant_htons(ETH_P_8021Q): {
		const struct vlan_hdr *vlan;
		struct vlan_hdr _vlan;

		vlan = skb_header_pointer(skb, nhoff, sizeof(_vlan), &_vlan);
		if (!vlan)
			return false;

		proto = vlan->h_vlan_encapsulated_proto;
		nhoff += sizeof(*vlan);
		goto again;
	}
	case __constant_htons(ETH_P_PPP_SES): {
		struct {
			struct pppoe_hdr hdr;
			__be16 proto;
		} *hdr, _hdr;
		hdr = skb_header_pointer(skb, nhoff, sizeof(_hdr), &_hdr);
		if (!hdr)
			return false;
		proto = hdr->proto;
		nhoff += PPPOE_SES_HLEN;
		switch (proto) {
		case __constant_htons(PPP_IP):
			goto ip;
		case __constant_htons(PPP_IPV6):
			goto ipv6;
		default:
			return false;
		}
	}
	default:
		return false;
	}

	switch (ip_proto) {
	case IPPROTO_GRE: {
		struct gre_hdr {
			__be16 flags;
			__be16 proto;
		} *hdr, _hdr;

		hdr = skb_header_pointer(skb, nhoff, sizeof(_hdr), &_hdr);
		if (!hdr)
			return false;
		/*
		 * Only look inside GRE if version zero and no
		 * routing
		 */
		if (!(hdr->flags & (GRE_VERSION|GRE_ROUTING))) {
			proto = hdr->proto;
			nhoff += 4;
			if (hdr->flags & GRE_CSUM)
				nhoff += 4;
			if (hdr->flags & GRE_KEY)
				nhoff += 4;
			if (hdr->flags & GRE_SEQ)
				nhoff += 4;
			goto again;
		}
		break;
	}
	case IPPROTO_IPIP:
		goto again;
	default:
		break;
	}

	flow->ip_proto = ip_proto;
	poff = proto_ports_offset(ip_proto);
	if (poff >= 0) {
		__be32 *ports, _ports;

		nhoff += poff;
		ports = skb_header_pointer(skb, nhoff, sizeof(_ports), &_ports);
		if (ports)
			flow->ports = *ports;
	}

	return true;
}
EXPORT_SYMBOL(skb_flow_dissect);
#define FCOE_HEADER_LEN 38
/**
 * __skb_flow_dissect - extract the flow_keys struct and return it
 * @skb: sk_buff to extract the flow from, can be NULL if the rest are specified
 * @data: raw buffer pointer to the packet, if NULL use skb->data
 * @proto: protocol for which to get the flow, if @data is NULL use skb->protocol
 * @nhoff: network header offset, if @data is NULL use skb_network_offset(skb)
 * @hlen: packet header length, if @data is NULL use skb_headlen(skb)
 *
 * The function will try to retrieve the struct flow_keys from either the skbuff
 * or a raw buffer specified by the rest parameters
 */
bool __skb_flow_dissect(const struct sk_buff *skb, struct flow_keys *flow,
			void *data, __be16 proto, int nhoff, int hlen)
{
	u8 ip_proto;

	if (!data) {
		data = skb->data;
		proto = skb->protocol;
		nhoff = skb_network_offset(skb);
		hlen = skb_headlen(skb);
	}

	memset(flow, 0, sizeof(*flow));

again:
	switch (proto) {
	case htons(ETH_P_IP): {
		const struct iphdr *iph;
		struct iphdr _iph;
ip:
		iph = __skb_header_pointer(skb, nhoff, sizeof(_iph), data, hlen, &_iph);
		if (!iph || iph->ihl < 5)
			return false;
		nhoff += iph->ihl * 4;

		ip_proto = iph->protocol;
		if (ip_is_fragment(iph))
			ip_proto = 0;

		/* skip the address processing if skb is NULL.  The assumption
		 * here is that if there is no skb we are not looking for flow
		 * info but lengths and protocols.
		 */
		if (!skb)
			break;

		iph_to_flow_copy_addrs(flow, iph);
		break;
	}
	case htons(ETH_P_IPV6): {
#if 0
		const struct ipv6hdr *iph;
		struct ipv6hdr _iph;
		__be32 flow_label;

ipv6:
		iph = __skb_header_pointer(skb, nhoff, sizeof(_iph), data, hlen, &_iph);
		if (!iph)
			return false;

		ip_proto = iph->nexthdr;
		nhoff += sizeof(struct ipv6hdr);

		/* see comment above in IPv4 section */
		if (!skb)
			break;

		flow->src = (__force __be32)ipv6_addr_hash(&iph->saddr);
		flow->dst = (__force __be32)ipv6_addr_hash(&iph->daddr);

		flow_label = ip6_flowlabel(iph);
		if (flow_label) {
			/* Awesome, IPv6 packet has a flow label so we can
			 * use that to represent the ports without any
			 * further dissection.
			 */
			flow->n_proto = proto;
			flow->ip_proto = ip_proto;
			flow->ports = flow_label;
			flow->thoff = (u16)nhoff;

			return true;
		}
#endif
		break;
	}
	case htons(ETH_P_8021AD):
	case htons(ETH_P_8021Q): {
		const struct vlan_hdr *vlan;
		struct vlan_hdr _vlan;

		vlan = __skb_header_pointer(skb, nhoff, sizeof(_vlan), data, hlen, &_vlan);
		if (!vlan)
			return false;

		proto = vlan->h_vlan_encapsulated_proto;
		nhoff += sizeof(*vlan);
		goto again;
	}
	case htons(ETH_P_PPP_SES): {
		struct {
			struct pppoe_hdr hdr;
			__be16 proto;
		} *hdr, _hdr;
		hdr = __skb_header_pointer(skb, nhoff, sizeof(_hdr), data, hlen, &_hdr);
		if (!hdr)
			return false;
		proto = hdr->proto;
		nhoff += PPPOE_SES_HLEN;
		switch (proto) {
		case htons(PPP_IP):
			goto ip;
		case htons(PPP_IPV6):
		//	goto ipv6;
		default:
			return false;
		}
	}
	case htons(ETH_P_FCOE):
		flow->thoff = (u16)(nhoff + FCOE_HEADER_LEN);
		/* fall through */
	default:
		return false;
	}

	switch (ip_proto) {
	case IPPROTO_GRE: {
		struct gre_hdr {
			__be16 flags;
			__be16 proto;
		} *hdr, _hdr;

		hdr = __skb_header_pointer(skb, nhoff, sizeof(_hdr), data, hlen, &_hdr);
		if (!hdr)
			return false;
		/*
		 * Only look inside GRE if version zero and no
		 * routing
		 */
		if (!(hdr->flags & (GRE_VERSION|GRE_ROUTING))) {
			proto = hdr->proto;
			nhoff += 4;
			if (hdr->flags & GRE_CSUM)
				nhoff += 4;
			if (hdr->flags & GRE_KEY)
				nhoff += 4;
			if (hdr->flags & GRE_SEQ)
				nhoff += 4;
			if (proto == htons(ETH_P_TEB)) {
				const struct ethhdr *eth;
				struct ethhdr _eth;

				eth = __skb_header_pointer(skb, nhoff,
							   sizeof(_eth),
							   data, hlen, &_eth);
				if (!eth)
					return false;
				proto = eth->h_proto;
				nhoff += sizeof(*eth);
			}
			goto again;
		}
		break;
	}
	case IPPROTO_IPIP:
		proto = htons(ETH_P_IP);
		goto ip;
	case IPPROTO_IPV6:
		proto = htons(ETH_P_IPV6);
		//goto ipv6;
	default:
		break;
	}

	flow->n_proto = proto;
	flow->ip_proto = ip_proto;
	flow->thoff = (u16) nhoff;

	/* unless skb is set we don't need to record port info */
	if (skb)
		flow->ports = __skb_flow_get_ports(skb, nhoff, ip_proto,
						   data, hlen);

	return true;
}
EXPORT_SYMBOL(__skb_flow_dissect);

#define STATIC_KEY_INIT_TRUE ((struct static_key)		\
	{ .enabled = ATOMIC_INIT(1),				\
	  .entries = (void *)JUMP_LABEL_TYPE_TRUE_BRANCH })
#define STATIC_KEY_INIT_FALSE ((struct static_key)		\
	{ .enabled = ATOMIC_INIT(0),				\
	  .entries = (void *)JUMP_LABEL_TYPE_FALSE_BRANCH })
#define net_get_random_once(buf, nbytes)				\
	({								\
		bool ___ret = false;					\
		static bool ___done = false;				\
		static struct static_key ___once_key =			\
			STATIC_KEY_INIT_TRUE;				\
		if (static_key_true(&___once_key))			\
			___ret = __net_get_random_once(buf,		\
						       nbytes,		\
						       &___done,	\
						       &___once_key);	\
		___ret;							\
	})
static u32 hashrnd __read_mostly;
static __always_inline void __flow_hash_secret_init(void)
{
//可能会有问题
//	net_get_random_once(&hashrnd, sizeof(hashrnd));
}

static __always_inline u32 __flow_hash_3words(u32 a, u32 b, u32 c)
{
	__flow_hash_secret_init();
	return jhash_3words(a, b, c, hashrnd);
}
static inline u32 __flow_hash_from_keys(struct flow_keys *keys)
{
	u32 hash;

	/* get a consistent hash (same value on both flow directions) */
	if (((__force u32)keys->dst < (__force u32)keys->src) ||
	    (((__force u32)keys->dst == (__force u32)keys->src) &&
	     ((__force u16)keys->port16[1] < (__force u16)keys->port16[0]))) {
		swap(keys->dst, keys->src);
		swap(keys->port16[0], keys->port16[1]);
	}

	hash = __flow_hash_3words((__force u32)keys->dst,
				  (__force u32)keys->src,
				  (__force u32)keys->ports);
	if (!hash)
		hash = 1;

	return hash;
}
static inline bool skb_flow_dissect_vxlan(const struct sk_buff *skb, struct flow_keys *flow)
{
	return __skb_flow_dissect(skb, flow, NULL, 0, 0, 0);
}
/*
 * __skb_get_hash: calculate a flow hash based on src/dst addresses
 * and src/dst port numbers.  Sets hash in skb to non-zero hash value
 * on success, zero indicates no valid hash.  Also, sets l4_hash in skb
 * if hash is a canonical 4-tuple hash over transport ports.
 */
void __skb_get_hash(struct sk_buff *skb)
{
	struct flow_keys keys;

	if (!skb_flow_dissect_vxlan(skb, &keys))
		return;

	if (keys.ports)
		skb->l4_hash = 1;

	skb->sw_hash = 1;

	skb->rxhash = __flow_hash_from_keys(&keys);
}
EXPORT_SYMBOL(__skb_get_hash);