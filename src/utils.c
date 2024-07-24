#include "ft_malcolm.h"

// parsing.c
int ft_ishexadecimal(int c)
{
	return (ft_isdigit(c) || (c >= 'a' && c <= 'f'));
}

size_t	get_array_length(char **arr)
{
	size_t i = -1;
	while (arr[++i]) { ; }
	return i;
}

// print utils and verbose mode
void print_mac(const uint8_t mac[MAC_LENGTH]) {
	printf("%02x:%02x:%02x:%02x:%02x:%02x", 
		   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void print_ip(const uint8_t ip[IPV4_LENGTH]) {
	printf("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

void verbose(t_arp_header arp)
{
	printf("ARP Header:\n");
	printf("  Hardware type: 0x%04x ", ntohs(arp.hardware_type));
	switch(ntohs(arp.hardware_type)) {
		case 1: printf("(Ethernet)\n"); break;
		default: printf("(Unknown)\n");
	}

	printf("  Protocol type: 0x%04x ", ntohs(arp.protocol_type));
	switch(ntohs(arp.protocol_type)) {
		case 0x0800: printf("(IPv4)\n"); break;
		default: printf("(Unknown)\n");
	}

	printf("  Hardware address length: %d\n", arp.hardware_len);
	printf("  Protocol address length: %d\n", arp.protocol_len);

	printf("  Opcode: %d ", ntohs(arp.opcode));
	switch(ntohs(arp.opcode)) {
		case 1: printf("(ARP Request)\n"); break;
		case 2: printf("(ARP Reply)\n"); break;
		default: printf("(Unknown)\n");
	}

	printf("  Sender MAC address: ");
	print_mac(arp.source.mac);
	printf("\n");

	printf("  Sender IP address: ");
	print_ip(arp.source.ip);
	printf("\n");

	printf("  Target MAC address: ");
	print_mac(arp.target.mac);
	printf("\n");

	printf("  Target IP address: ");
	print_ip(arp.target.ip);
	printf("\n");
}
