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

#include <linux/if_packet.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <linux/if_link.h>

#define ETHERNET 1
#define REQUEST 1
#define BROADCAST 1
#define REPLY 2
#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define BUFF_SIZE 65536

typedef struct s_client
{
	uint8_t ip[IPV4_LENGTH];
	uint8_t mac[MAC_LENGTH];
} t_client;

typedef struct s_eth_header
{
	uint8_t destination_mac[MAC_LENGTH];
	uint8_t source_mac[MAC_LENGTH];
	uint16_t ether_type;
} __attribute__((packed)) t_eth_header;

typedef struct s_arp_header
{
	uint16_t protocol_type;
	uint16_t hardware_type;
	uint8_t hardware_len;
	uint8_t protocol_len;
	uint16_t opcode;
	t_client source;
	t_client target;
} __attribute__((packed)) t_arp_header;

typedef struct s_arp_packet
{
	t_arp_header arp_header;
	t_eth_header eth_header;
} t_arp_packet;

typedef struct s_malcolm
{
	int sockfd;
	t_arp_packet packet;
	char if_name[IFNAMSIZ];
	struct sockaddr_ll sll;
} t_malcolm;

// sockets.c
int initialize_socket(t_malcolm *malcolm);
// packets.c
void prepare_broadcast_packet(t_malcolm *malcolm, const t_client *source, const t_client *target);
int send_arp_packet(t_malcolm *malcolm);
// parsing.c
int parse_arguments(char **argv, t_client *source, t_client *target);
int	parse_mac_address(const char *str, uint8_t (*mac)[MAC_LENGTH]);
int	parse_ip_address(const char *str, uint8_t (*ip)[IPV4_LENGTH]);
// utils.c
int ft_ishexadecimal(int c);
size_t get_array_length(char **arr);
void verbose(const t_malcolm *malcolm);
void print_mac(const uint8_t *mac);
void print_ip(const uint8_t *ip);

#endif
