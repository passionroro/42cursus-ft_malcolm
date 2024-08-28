#include "ft_malcolm.h"

int run(t_malcolm *malcolm)
{
	int ok;

	if (initialize_socket(malcolm) < 0)
	{
		// socket already closed here
		return -1;
	}

	socklen_t addrlen = sizeof(struct sockaddr_ll);

	while (1)
	{
		unsigned char buffer[BUFF_SIZE] = {0};
		ssize_t bytes_read = recvfrom(malcolm->sockfd, buffer, BUFF_SIZE, 0, (struct sockaddr *)&malcolm->sll, &addrlen);
		if (bytes_read < -1)
		{
			ok = handle_error("recvfrom");
			break;
		}
		else if (bytes_read <= 0)
		{
			break;
		}
		else if (bytes_read > 0)
		{
			struct ethhdr *eth = (struct ethhdr *)buffer;
			if (ntohs(eth->h_proto) == ETH_P_ARP)
			{
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
					ok = send_spoofed_arp(malcolm);
					break;
				}
			}
		}
	}

	close(malcolm->sockfd);

	printf("good-bye!\n");

	return ok;
}

int main(int argc, char **argv)
{
	t_client source, target;
	t_malcolm malcolm = {0};

	if (argc < 5 || argc > 6)
	{
		print_help(argv[0]);
		return -1;
	}

	if (parse_arguments(argv, &source, &target, &malcolm.verbose) != 0)
	{
		return -1;
	}

	malcolm.source = source;
	malcolm.target = target;

	return run(&malcolm);
}
