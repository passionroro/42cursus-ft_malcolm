#ifndef __FT_MALCOLM_H__
#define __FT_MALCOLM_H__

#include "../libft/libft.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <limits.h>

#define HARDWARE 1
#define ETHER 0x0806
#define PROTOCOL 0x0800
#define REQUEST 1
#define REPLY 2
#define MAC_LENGTH 6
#define IPV4_LENGTH 4


typedef struct s_client
{
	uint8_t	ip[IPV4_LENGTH];
	uint8_t	mac[MAC_LENGTH];
}	t_client;

typedef struct s_eth_header
{
	uint8_t	destination_mac[MAC_LENGTH];
	uint8_t	source_mac[MAC_LENGTH];
	uint16_t	ether_type;
}	t_eth_header;

typedef struct s_arp_header {
	t_eth_header	eth_header;
	uint16_t	protocol_type;
	uint16_t	hardware_type;
	uint8_t		hardware_len;
	uint8_t		protocol_len;
	uint16_t	opcode;
	t_client	source;
	t_client	target;
} __attribute__((packed)) t_arp_header;


// parsing.c
int	parse_arguments(char **argv, t_arp_header *arp);
// utils.c
int		ft_ishexadecimal(int c);
size_t	get_array_length(char **arr);
void	verbose(t_arp_header arp);

#endif
