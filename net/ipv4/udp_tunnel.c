#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/capability.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_arp.h>
#include <linux/mroute.h>
#include <linux/init.h>
#include <linux/in6.h>
#include <linux/inetdevice.h>
#include <linux/igmp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/rculist.h>
#include <linux/err.h>
#include <linux/hash.h>

#include <net/sock.h>
#include <net/ip.h>
#include <net/icmp.h>
#include <net/protocol.h>
#include <net/ip_tunnels.h>
#include <net/arp.h>
#include <net/checksum.h>
#include <net/dsfield.h>
#include <net/inet_ecn.h>
#include <net/xfrm.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/rtnetlink.h>
#include <net/udp.h>
#include <net/gue.h>

#include <linux/errno.h>
#include <net/udp_tunnel.h>

//#if IS_ENABLED(CONFIG_IPV6)
#if 0
#include <net/ipv6.h>
#include <net/ip6_fib.h>
#include <net/ip6_route.h>
#endif
/****************************************************************************************************/
/************************************   udp.c    ************************************************/
/****************************************************************************************************/

/* Function to set UDP checksum for an IPv4 UDP packet. This is intended
 * for the simple case like when setting the checksum for a UDP tunnel.
 */
void udp_set_csum(bool nocheck, struct sk_buff *skb,
		  __be32 saddr, __be32 daddr, int len)
{
	struct udphdr *uh = udp_hdr(skb);

	if (nocheck)
		uh->check = 0;
	else if (skb_is_gso(skb))
		uh->check = ~udp_v4_check(len, saddr, daddr, 0);
	else if (skb_dst(skb) && skb_dst(skb)->dev &&
		 (skb_dst(skb)->dev->features & NETIF_F_V4_CSUM)) {

		BUG_ON(skb->ip_summed == CHECKSUM_PARTIAL);

		skb->ip_summed = CHECKSUM_PARTIAL;
		skb->csum_start = skb_transport_header(skb) - skb->head;
		skb->csum_offset = offsetof(struct udphdr, check);
		uh->check = ~udp_v4_check(len, saddr, daddr, 0);
	} else {
		__wsum csum;

		BUG_ON(skb->ip_summed == CHECKSUM_PARTIAL);

		uh->check = 0;
		csum = skb_checksum(skb, 0, len, 0);
		uh->check = udp_v4_check(len, saddr, daddr, csum);
		if (uh->check == 0)
			uh->check = CSUM_MANGLED_0;

		skb->ip_summed = CHECKSUM_UNNECESSARY;
	}
}
//static struct static_key udpv6_encap_needed __read_mostly;
static struct jump_label_key udpv6_encap_needed __read_mostly;
void udpv6_encap_enable(void)
{
/*	
	此函数 应在 ipv6/udp.c 中，此处调试 暂放此处
  	tihuan  原 kernel 3.18.45 中的函数 如下
*/
 /*       if (!static_key_enabled(&udpv6_encap_needed))
                static_key_slow_inc(&udpv6_encap_needed);
 */
   if (!jump_label_enabled(&udpv6_encap_needed))
   		jump_label_inc(&udpv6_encap_needed);
}
//EXPORT_SYMBOL(udpv6_encap_enable);

//static struct static_key udp_encap_needed __read_mostly;
static struct jump_label_key udp_encap_needed __read_mostly;
void udp_encap_enable(void)
{
/* tihuan  原 kernel 3.18.45 中的函数 如下
	if (!static_key_enabled(&udp_encap_needed))
		static_key_slow_inc(&udp_encap_needed);
*/
	if (!jump_label_enabled(&udp_encap_needed))
		jump_label_inc(&udp_encap_needed);
}
//EXPORT_SYMBOL(udp_encap_enable);

/****************************************************************************************************/
/************************************   udp.c    ************************************************/
/****************************************************************************************************/

/****************************************************************************************************/
/************************************   ip_tunnel.c    ************************************************/
/****************************************************************************************************/
/****************************************************************************************************/
/************************************   sk_buff.c    ************************************************/
/****************************************************************************************************/
/**
 * skb_scrub_packet - scrub an skb
 *
 * @skb: buffer to clean
 * @xnet: packet is crossing netns
 *
 * skb_scrub_packet can be used after encapsulating or decapsulting a packet
 * into/from a tunnel. Some information have to be cleared during these
 * operations.
 * skb_scrub_packet can also be used to clean a skb before injecting it in
 * another namespace (@xnet == true). We have to clear all information in the
 * skb that could impact namespace isolation.
 */
void skb_scrub_packet(struct sk_buff *skb, bool xnet)
{
	skb->tstamp.tv64 = 0;
	skb->pkt_type = PACKET_HOST;
	skb->skb_iif = 0;
	skb->ignore_df = 0;
	skb_dst_drop(skb);
	secpath_reset(skb);
	nf_reset(skb);
	nf_reset_trace(skb);

	if (!xnet)
		return;

	skb_orphan(skb);
	skb->mark = 0;
}
EXPORT_SYMBOL_GPL(skb_scrub_packet);
/************************************   sk_buff.c    ************************************************/
/****************************************************************************************************/
/************************************ ip tunnel core ************************************************/
/************************************ ip tunnel core ************************************************/
/* line number 49*/
int iptunnel_xmit(struct sock *sk, struct rtable *rt, struct sk_buff *skb,
		  __be32 src, __be32 dst, __u8 proto,
		  __u8 tos, __u8 ttl, __be16 df, bool xnet)
{
	int pkt_len = skb->len;
	struct iphdr *iph;
	int err;

	skb_scrub_packet(skb, xnet);//清空skb相关信息  

	skb_clear_hash(skb); //清空skb hash值 
	skb_dst_set(skb, &rt->dst); //rt必须根据外层报文的IP地址及相关信息获取到的  
	memset(IPCB(skb), 0, sizeof(*IPCB(skb)));

	/* Push down and install the IP header. */
	skb_push(skb, sizeof(struct iphdr));
	skb_reset_network_header(skb);

	iph = ip_hdr(skb);

	iph->version	=	4;
	iph->ihl	=	sizeof(struct iphdr) >> 2;
	iph->frag_off	=	df;//OVS2.5默认有df标记  
	iph->protocol	=	proto; //vxlan，该协议为UDP  
	iph->tos	=	tos;
	iph->daddr	=	dst;
	iph->saddr	=	src;
	iph->ttl	=	ttl;
#if 0
	/* 原始函数*/
	//计算IP报文的ID值  
	//写入到 iph->id 
	__ip_select_ident_vxlan(iph, skb_shinfo(skb)->gso_segs ?: 1);
#else
	/* 调试函数*/
	__ip_select_ident(iph,&rt->dst,0);
#endif	
	/* 修改 tihuan*/
	//err = ip_local_out_sk(sk, skb);
	//  //调用ip层发送函数
	err = ip_local_out(skb);
	if (unlikely(net_xmit_eval(err)))
		pkt_len = 0;
	return pkt_len;
}
EXPORT_SYMBOL_GPL(iptunnel_xmit);
/* line number 86*/
int iptunnel_pull_header(struct sk_buff *skb, int hdr_len, __be16 inner_proto)
{
	if (unlikely(!pskb_may_pull(skb, hdr_len)))
		return -ENOMEM;

	skb_pull_rcsum(skb, hdr_len);

	if (inner_proto == htons(ETH_P_TEB)) {
		struct ethhdr *eh;

		if (unlikely(!pskb_may_pull(skb, ETH_HLEN)))
			return -ENOMEM;

		eh = (struct ethhdr *)skb->data;
		if (likely(ntohs(eh->h_proto) >= ETH_P_802_3_MIN))
			skb->protocol = eh->h_proto;
		else
			skb->protocol = htons(ETH_P_802_2);

	} else {
		skb->protocol = inner_proto;
	}

	nf_reset(skb);
	secpath_reset(skb);
	skb_clear_hash_if_not_l4(skb);
	skb_dst_drop(skb);
	skb->vlan_tci = 0;
	skb_set_queue_mapping(skb, 0);
	skb->pkt_type = PACKET_HOST;
	return 0;
}
EXPORT_SYMBOL_GPL(iptunnel_pull_header);

/* line number 120*/
struct sk_buff *iptunnel_handle_offloads(struct sk_buff *skb,
					 bool csum_help,
					 int gso_type_mask)
{
	int err;

	if (likely(!skb->encapsulation)) {
		skb_reset_inner_headers(skb);
		skb->encapsulation = 1;
	}

	if (skb_is_gso(skb)) {
		err = skb_unclone(skb, GFP_ATOMIC);
		if (unlikely(err))
			goto error;
		skb_shinfo(skb)->gso_type |= gso_type_mask;
		return skb;
	}

	/* If packet is not gso and we are resolving any partial checksum,
	 * clear encapsulation flag. This allows setting CHECKSUM_PARTIAL
	 * on the outer header without confusing devices that implement
	 * NETIF_F_IP_CSUM with encapsulation.
	 */
	if (csum_help)
		skb->encapsulation = 0;

	if (skb->ip_summed == CHECKSUM_PARTIAL && csum_help) {
		err = skb_checksum_help(skb);
		if (unlikely(err))
			goto error;
	} else if (skb->ip_summed != CHECKSUM_PARTIAL)
		skb->ip_summed = CHECKSUM_NONE;

	return skb;
error:
	kfree_skb(skb);
	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(iptunnel_handle_offloads);

/* line number 162*/
/* Often modified stats are per cpu, other are shared (netdev->stats) */
struct rtnl_link_stats64 *ip_tunnel_get_stats64(struct net_device *dev,
						struct rtnl_link_stats64 *tot)
{
	int i;

	for_each_possible_cpu(i) {
		const struct pcpu_sw_netstats *tstats =
						   per_cpu_ptr(dev->tstats, i);
		u64 rx_packets, rx_bytes, tx_packets, tx_bytes;
		unsigned int start;

		do {
/* tihuan */
			//start = u64_stats_fetch_begin_irq(&tstats->syncp);
			start = u64_stats_fetch_begin_bh(&tstats->syncp);
			rx_packets = tstats->rx_packets;
			tx_packets = tstats->tx_packets;
			rx_bytes = tstats->rx_bytes;
			tx_bytes = tstats->tx_bytes;
		} while (u64_stats_fetch_retry_bh(&tstats->syncp, start));
/* tihuan */ //		} while (u64_stats_fetch_retry_irq(&tstats->syncp, start));

		tot->rx_packets += rx_packets;
		tot->tx_packets += tx_packets;
		tot->rx_bytes   += rx_bytes;
		tot->tx_bytes   += tx_bytes;
	}

	tot->multicast = dev->stats.multicast;

	tot->rx_crc_errors = dev->stats.rx_crc_errors;
	tot->rx_fifo_errors = dev->stats.rx_fifo_errors;
	tot->rx_length_errors = dev->stats.rx_length_errors;
	tot->rx_frame_errors = dev->stats.rx_frame_errors;
	tot->rx_errors = dev->stats.rx_errors;

	tot->tx_fifo_errors = dev->stats.tx_fifo_errors;
	tot->tx_carrier_errors = dev->stats.tx_carrier_errors;
	tot->tx_dropped = dev->stats.tx_dropped;
	tot->tx_aborted_errors = dev->stats.tx_aborted_errors;
	tot->tx_errors = dev->stats.tx_errors;

	tot->collisions  = dev->stats.collisions;

	return tot;
}
EXPORT_SYMBOL_GPL(ip_tunnel_get_stats64);
/****************************************************************************************************/
/************************************   ip_tunnel.c     ************************************************/
/****************************************************************************************************/

int udp_sock_create4(struct net *net, struct udp_port_cfg *cfg,
		     struct socket **sockp)
{
	int err;
	struct socket *sock = NULL;
	struct sockaddr_in udp_addr;

	err = sock_create_kern(AF_INET, SOCK_DGRAM, 0, &sock);
	if (err < 0)
		goto error;

	sk_change_net(sock->sk, net);

	udp_addr.sin_family = AF_INET;
	udp_addr.sin_addr = cfg->local_ip;
	udp_addr.sin_port = cfg->local_udp_port;
	err = kernel_bind(sock, (struct sockaddr *)&udp_addr,
			  sizeof(udp_addr));
	if (err < 0)
		goto error;

	if (cfg->peer_udp_port) {
		udp_addr.sin_family = AF_INET;
		udp_addr.sin_addr = cfg->peer_ip;
		udp_addr.sin_port = cfg->peer_udp_port;
		err = kernel_connect(sock, (struct sockaddr *)&udp_addr,
				     sizeof(udp_addr), 0);
		if (err < 0)
			goto error;
	}

/*修改 tihuan*/	
	//sock->sk->sk_no_check_tx = !cfg->use_udp_checksums;
	sock->sk->sk_no_check = !cfg->use_udp_checksums;
	*sockp = sock;
	return 0;

error:
	if (sock) {
		kernel_sock_shutdown(sock, SHUT_RDWR);
		sk_release_kernel(sock->sk);
	}
	*sockp = NULL;
	return err;
}
EXPORT_SYMBOL(udp_sock_create4);

void setup_udp_tunnel_sock(struct net *net, struct socket *sock,
			   struct udp_tunnel_sock_cfg *cfg)
{
	struct sock *sk = sock->sk;

	/* Disable multicast loopback */
	inet_sk(sk)->mc_loop = 0;

	/* Enable CHECKSUM_UNNECESSARY to CHECKSUM_COMPLETE conversion */
	udp_set_convert_csum(sk, true);

	rcu_assign_sk_user_data(sk, cfg->sk_user_data);

	udp_sk(sk)->encap_type = cfg->encap_type;
	udp_sk(sk)->encap_rcv = cfg->encap_rcv;
	udp_sk(sk)->encap_destroy = cfg->encap_destroy;

//	udp_tunnel_encap_enable(sock);
//#if IS_ENABLED(CONFIG_IPV6)
#if 0
	if (sock->sk->sk_family == PF_INET6)
		ipv6_stub->udpv6_encap_enable();
	else
#endif
	udp_encap_enable();
}
EXPORT_SYMBOL_GPL(setup_udp_tunnel_sock);

int udp_tunnel_xmit_skb(struct socket *sock, struct rtable *rt,
			struct sk_buff *skb, __be32 src, __be32 dst,
			__u8 tos, __u8 ttl, __be16 df, __be16 src_port,
			__be16 dst_port, bool xnet)
{
	struct udphdr *uh;

	__skb_push(skb, sizeof(*uh));
	skb_reset_transport_header(skb);
	uh = udp_hdr(skb);

	uh->dest = dst_port;
	uh->source = src_port;
	uh->len = htons(skb->len);
/*修改 tihuan*/	
	//udp_set_csum(sock->sk->sk_no_check_tx, skb, src, dst, skb->len);
	udp_set_csum(sock->sk->sk_no_check, skb, src, dst, skb->len);
/*
iptunnel_xmit函数是发送vxlan报文时，封装UDP报文头之后被调用的，主要作用是封装IP头，并调用三层发包函数，完成报文的发送，该函数相对比较简单。
*/
	return iptunnel_xmit(sock->sk, rt, skb, src, dst, IPPROTO_UDP,
			     tos, ttl, df, xnet);
}
EXPORT_SYMBOL_GPL(udp_tunnel_xmit_skb);

void udp_tunnel_sock_release(struct socket *sock)
{
	rcu_assign_sk_user_data(sock->sk, NULL);
	kernel_sock_shutdown(sock, SHUT_RDWR);
	sk_release_kernel(sock->sk);
}
EXPORT_SYMBOL_GPL(udp_tunnel_sock_release);

MODULE_LICENSE("GPL");
