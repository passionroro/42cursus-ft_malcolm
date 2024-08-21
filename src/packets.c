#include "ft_malcolm.h"
/*
void create_arp_header(t_arp_header *arp, const t_client *source, const t_client *target)
{
	arp->hardware_type = htons(ETHERNET);
	arp->protocol_type = htons(ETH_P_IP);
	arp->hardware_len = MAC_LENGTH;
	arp->protocol_len = IPV4_LENGTH;
	arp->opcode = htons(REQUEST);
	ft_memcpy(&arp->source, source, sizeof(t_client));
	ft_memcpy(&arp->target, target, sizeof(t_client));
}

void create_broadcast_eth_header(t_eth_header *eth_header, const uint8_t *source_mac)
{
	ft_memset(eth_header->destination_mac, 0xFF, MAC_LENGTH);
	ft_memcpy(eth_header->source_mac, source_mac, MAC_LENGTH);
	eth_header->ether_type = htons(ETH_P_ARP);
}

void prepare_broadcast_packet(t_malcolm *malcolm, const t_client *source, const t_client *target)
{
	create_broadcast_eth_header(&malcolm->packet.eth_header, source->mac);
	create_arp_header(&malcolm->packet.arp_header, source, target);
}

int send_arp_packet(t_malcolm *malcolm)
{
	if (sendto(malcolm->sockfd, &malcolm->packet, sizeof(malcolm->packet), 0,
			   (struct sockaddr*)&malcolm->sll, sizeof(malcolm->sll)) < 0)
	{
		close(malcolm->sockfd);
		return (handle_error("sendto"));
	}
	return 0;
}
*/
