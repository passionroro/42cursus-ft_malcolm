#include "ft_malcolm.h"

// parsing.c
int ft_ishexadecimal(int c)
{
	return (ft_isdigit(c) || (c >= 'a' && c <= 'f'));
}

size_t get_array_length(char **arr)
{
	size_t i = -1;
	while (arr[++i])
	{
		;
	}
	return i;
}

// print utils and verbose mode
void print_mac(const uint8_t *mac)
{
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
		   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void print_ip(const uint8_t *ip)
{
	printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
}
void print_eth_header(struct ethhdr *eth)
{
	printf("  Ethernet Header:\n");
	printf("	Destination MAC: ");
	print_mac(eth->h_dest);
	printf("	Source MAC: ");
	print_mac(eth->h_source);
	printf("	Type: %s (0x%04x)\n", ntohs(eth->h_proto) == 0x0806 ? "ARP" : "Unknown", ntohs(eth->h_proto));
}

void print_arp_header(t_arphdr *arp)
{
	printf("  ARP Header:\n");
	printf("	Hardware type: %s (0x%04x)\n", ntohs(arp->ar_hrd) == ARPHRD_ETHER ? "Ethernet" : "Unknown", ntohs(arp->ar_hrd));
	printf("	Protocol type: IPv4 (0x%04x)\n", ntohs(arp->ar_pro));
	printf("	Hardware address length: %d\n", arp->ar_hln);
	printf("	Protocol address length: %d\n", arp->ar_pln);
	printf("	Opcode: %d (%s)\n", ntohs(arp->ar_op),
		   ntohs(arp->ar_op) == REQUEST ? "REQUEST" : "REPLY");
	printf("	Sender MAC address: ");
	print_mac(arp->ar_sha);
	printf("	Sender IP address: ");
	print_ip(arp->ar_sip);
	printf("	Target MAC address: ");
	print_mac(arp->ar_tha);
	printf("	Target IP address: ");
	print_ip(arp->ar_tip);
}

void print_arp_packet(t_arp_packet packet)
{
	print_eth_header(packet.eth);
	print_arp_header(packet.arp);
}

void verbose(const t_malcolm *malcolm)
{
	printf("\n-----===============-----\n");
	printf("Malcolm Structure Details:\n");
	printf("Socket FD: %d\n", malcolm->sockfd);

	printf("ARP Packet:\n");
	print_arp_packet(malcolm->packet);

	printf("Socket Address:\n");
	printf("  Family: %u\n", malcolm->sll.sll_family);
	printf("  Protocol: 0x%04x\n", ntohs(malcolm->sll.sll_protocol));
	printf("  Interface Index: %d\n", malcolm->sll.sll_ifindex);
	printf("  Hardware Address Type: %u\n", malcolm->sll.sll_hatype);
	printf("  Packet Type: %u\n", malcolm->sll.sll_pkttype);
	printf("  Hardware Address Length: %u\n", malcolm->sll.sll_halen);
	printf("  Hardware Address: ");
	print_mac(malcolm->sll.sll_addr);
	printf("-----===============-----\n\n");
}

void print_help(char *arg)
{
	printf("ft_malcolm, by rohoarau for 42Lausanne\n");
	printf("usage: %s [ source ip ] [ source mac address ]\n\t\t    [ target ip ] [ target mac address ]\n", arg);
	printf("\t\t    [ -v <verbose> ]\n\n");
	printf("For complete usage info, check the README.\n");
}

bool is_broadcast_request(t_arp_packet *packet)
{
	return ft_memcmp(packet->eth->h_dest, "\xff\xff\xff\xff\xff\xff", ETH_ALEN) == 0;
}

bool is_request(t_arp_packet *packet)
{
	return ntohs(packet->arp->ar_op) == ARPOP_REQUEST;
}

bool is_from_target(t_malcolm *malcolm, t_arp_packet *packet)
{
	return ft_memcmp(packet->arp->ar_sip, malcolm->target.ip, 4) == 0 &&
		   ft_memcmp(packet->arp->ar_sha, malcolm->target.mac, ETH_ALEN) == 0;
}

bool is_requesting_source(t_malcolm *malcolm, t_arp_packet *packet)
{
	return ft_memcmp(packet->arp->ar_tip, malcolm->source.ip, 4) == 0;
}
