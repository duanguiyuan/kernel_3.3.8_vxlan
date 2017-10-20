#include <linux/module.h>
#include <linux/errno.h>
#include <linux/socket.h>
#include <linux/udp.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <net/udp.h>
#include <net/udp_tunnel.h>
#include <net/net_namespace.h>
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
