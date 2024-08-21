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
	printf("%02x:%02x:%02x:%02x:%02x:%02x",
		   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void print_ip(const uint8_t *ip)
{
	printf("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

void print_eth_header(const t_eth_header *eth)
{
	printf("Ethernet Header:\n");
	printf("  Destination MAC: ");
	print_mac(eth->destination_mac);
	printf("\n  Source MAC: ");
	print_mac(eth->source_mac);
	printf("\n  EtherType: 0x%04x\n", ntohs(eth->ether_type));
}

void print_arp_header(const t_arp_header *arp)
{
	printf("ARP Header:\n");
	printf("  Hardware type: 0x%04x\n", ntohs(arp->hardware_type));
	printf("  Protocol type: 0x%04x\n", ntohs(arp->protocol_type));
	printf("  Hardware address length: %d\n", arp->hardware_len);
	printf("  Protocol address length: %d\n", arp->protocol_len);
	printf("  Opcode: %d (%s)\n", ntohs(arp->opcode),
		   ntohs(arp->opcode) == REQUEST ? "REQUEST" : "REPLY");

	printf("  Source IP: ");
	print_ip(arp->source.ip);
	printf("\n  Source MAC: ");
	print_mac(arp->source.mac);

	printf("\n  Target IP: ");
	print_ip(arp->target.ip);
	printf("\n  Target MAC: ");
	print_mac(arp->target.mac);
	printf("\n");
}

void print_arp_packet(const t_arp_packet *packet)
{
	print_eth_header(&packet->eth_header);
	print_arp_header(&packet->arp_header);
}

void verbose(const t_malcolm *malcolm)
{
	printf("Malcolm Structure Details:\n");
	printf("Socket FD: %d\n", malcolm->sockfd);

	printf("\nARP Packet:\n");
	print_arp_packet(&malcolm->packet);

	printf("\nSocket Address:\n");
	printf("  Family: %u\n", malcolm->sll.sll_family);
	printf("  Protocol: 0x%04x\n", ntohs(malcolm->sll.sll_protocol));
	printf("  Interface Index: %d\n", malcolm->sll.sll_ifindex);
	printf("  Hardware Address Type: %u\n", malcolm->sll.sll_hatype);
	printf("  Packet Type: %u\n", malcolm->sll.sll_pkttype);
	printf("  Hardware Address Length: %u\n", malcolm->sll.sll_halen);
	printf("  Hardware Address: ");
	print_mac(malcolm->sll.sll_addr);
	printf("\n");
}