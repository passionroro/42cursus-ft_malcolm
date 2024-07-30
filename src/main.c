#include "ft_malcolm.h"

void	create_and_broadcast_arp(t_arp_header *arp)
{
	t_eth_header	eth_header;

	// creating Ethernet Frame Header
	ft_memset(eth_header.destination_mac, 0xFF, MAC_LENGTH);
	ft_memcpy(eth_header.source_mac, arp->source.mac, MAC_LENGTH);
	eth_header.ether_type = htons(ETHER);

	// finishing the creation of the ARP header
	arp->eth_header = eth_header;
	arp->hardware_type = htons(HARDWARE);
	arp->protocol_type = htons(PROTOCOL);
	arp->hardware_len = MAC_LENGTH;
	arp->protocol_len = IPV4_LENGTH;
	arp->opcode = htons(REQUEST);

	// target broadcasts an ARP request on the network
	// "Who has the IP source address ? Tell target." = trying to find MAC of source
	/*
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, NULL, NULL);

	struct sockaddr_ll socket_address;
	sendto(sock, arp, sizeof(arp), 0, (struct sockaddr*)&socket_address, sizeof(socket_address));
	*/
}

void	run(void)
{
	// program waits for this broadcast
	// when request is detected, prepare to send a spoofed reply
	// program sends ARP reply to the target
	// "<Source IP> is <Source MAC> (spoofed!)"
	// update ARP table
}

int	main(int argc, char **argv)
{
	t_arp_header	arp;

	if (argc != 5)
	{
		printf("Usage: %s [source ip] [source mac address] [target ip] [target mac address]\n", argv[0]);
		return -1;
	}

	if (parse_arguments(argv, &arp) != 0)
	{
		return -1;
	}

	create_and_broadcast_arp(&arp);

	verbose(arp);

	run();

	return 0;
}

