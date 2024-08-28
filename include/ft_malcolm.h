#ifndef __FT_MALCOLM_H__
#define __FT_MALCOLM_H__

#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define BG_BLACK "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GREEN "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN "\x1b[46m"
#define BG_WHITE "\x1b[47m"
#define RESET "\x1b[0m"

#include "../libft/libft.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <limits.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
// /usr/include/linux
#include <linux/if_packet.h>
#include <linux/if_link.h>
#include <linux/if_arp.h>

#define IPV4_LENGTH 4
#define BUFF_SIZE 65536
#define ETH_FRAME_MIN 60

typedef struct s_client
{
	uint8_t ip[IPV4_LENGTH];
	uint8_t mac[ETH_ALEN];
} t_client;

typedef struct s_arphdr
{
	__be16 ar_hrd;					/*format of hardware address*/
	__be16 ar_pro;					/*format of protocol address*/
	unsigned char ar_hln;			/*length of hardware address*/
	unsigned char ar_pln;			/*length of protocol address*/
	__be16 ar_op;					/*ARP opcode (command)*/
	unsigned char ar_sha[ETH_ALEN]; /*sender hardware address*/
	unsigned char ar_sip[4];		/*sender IP address*/
	unsigned char ar_tha[ETH_ALEN]; /*target hardware address*/
	unsigned char ar_tip[4];		/*target IP address*/
} t_arphdr;

typedef struct s_arp_packet
{
	struct ethhdr *eth;
	t_arphdr *arp;
} t_arp_packet;

typedef struct s_malcolm
{
	int sockfd;
	t_arp_packet packet;
	char if_name[IFNAMSIZ];
	struct sockaddr_ll sll;
	t_client source;
	t_client target;
	bool verbose;
} t_malcolm;

/**
 * FUNCTIONS
 */

// sockets.c
int initialize_socket(t_malcolm *malcolm);

// packets.c
int send_spoofed_arp(t_malcolm *malcolm);

// parsing.c
int parse_arguments(char **argv, t_client *source, t_client *target, bool *verbose);

// utils/utils.c
int ft_ishexadecimal(int c);
size_t get_array_length(char **arr);
bool is_broadcast_request(t_arp_packet *packet);
bool is_request(t_arp_packet *packet);
bool is_from_target(t_malcolm *malcolm, t_arp_packet *packet);
bool is_requesting_source(t_malcolm *malcolm, t_arp_packet *packet);

// utils/print.c
void verbose(const t_malcolm *malcolm);
void print_mac(const uint8_t *mac);
void print_ip(const uint8_t *ip);
void print_arp_packet(t_arp_packet packet);
void print_help(char *arg);

#endif // __FT_MALCOLM_H__
