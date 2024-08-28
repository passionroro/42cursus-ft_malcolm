#ifndef __FT_MALCOLM_H__
#define __FT_MALCOLM_H__

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

// /usr/include/linux/if_arp.h
#include <linux/if_packet.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <linux/if_arp.h>

#define ETHERNET 1
#define REQUEST 1
#define BROADCAST 1
#define REPLY 2
#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define BUFF_SIZE 65536
#define ETH_FRAME_MIN 60

typedef struct s_client
{
	uint8_t ip[IPV4_LENGTH];
	uint8_t mac[MAC_LENGTH];
} t_client;

typedef struct s_arphdr
{
	__be16	ar_hrd; /*format of hardware address*/
	__be16	ar_pro; /*format of protocol address*/
	unsigned char ar_hln; /*length of hardware address*/
	unsigned char ar_pln; /*length of protocol address*/
	__be16 ar_op; /*ARP opcode (command)*/
	unsigned char ar_sha[ETH_ALEN]; /*sender hardware address*/
	unsigned char ar_sip[4]; /*sender IP address*/
	unsigned char ar_tha[ETH_ALEN]; /*target hardware address*/
	unsigned char ar_tip[4]; /*target IP address*/
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

// sockets.c
int initialize_socket(t_malcolm *malcolm);
// packets.c
void prepare_broadcast_packet(t_malcolm *malcolm, const t_client *source, const t_client *target);
int send_arp_packet(t_malcolm *malcolm);
// parsing.c
int parse_arguments(char **argv, t_client *source, t_client *target, bool *verbose);
// utils.c
int ft_ishexadecimal(int c);
size_t get_array_length(char **arr);
void verbose(const t_malcolm *malcolm);
void print_mac(const uint8_t *mac);
void print_ip(const uint8_t *ip);
void print_arp_packet(t_arp_packet packet);
void print_help(char *arg);

bool is_broadcast_request(t_arp_packet *packet);
bool is_request(t_arp_packet *packet);
bool is_from_target(t_malcolm *malcolm, t_arp_packet *packet);
bool is_requesting_source(t_malcolm *malcolm, t_arp_packet *packet);

int send_spoofed_arp(t_malcolm *malcolm);

#endif
