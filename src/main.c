#include "ft_malcolm.h"

int run(t_malcolm *malcolm, const t_client *source, t_client *target)
{
	(void)source, (void)target;
	if (initialize_socket(malcolm) < 0)
		return -1;

	socklen_t addrlen = sizeof(struct sockaddr_ll);

	while (1)
	{
		unsigned char buffer[BUFF_SIZE] = {0};
		ssize_t bytes_read = recvfrom(malcolm->sockfd, buffer, BUFF_SIZE, 0, (struct sockaddr *)&malcolm->sll, &addrlen);
		if (bytes_read < -1)
		{
			return handle_error("recvfrom");
		}
		else if (bytes_read < 0)
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
				verbose(malcolm);
			}
		}
	}

	close(malcolm->sockfd);

	return 0;
}

int main(int argc, char **argv)
{
	t_client source, target;
	t_malcolm malcolm = {0};

	if (argc != 5)
	{
		printf("Usage: %s [source ip] [source mac address] [target ip] [target mac address]\n", argv[0]);
		return -1;
	}

	if (parse_arguments(argv, &source, &target) != 0)
	{
		return -1;
	}

	return run(&malcolm, &source, &target);
}

