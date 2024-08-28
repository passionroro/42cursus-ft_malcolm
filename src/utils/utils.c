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
