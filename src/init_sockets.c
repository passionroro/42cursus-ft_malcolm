#include "ft_malcolm.h"

int set_socket_timeout(int sockfd, int seconds, int microseconds)
{
	struct timeval timeout;
	timeout.tv_sec = seconds;
	timeout.tv_usec = microseconds;

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
	{
		errno = ENOMEM;
		return -1;
	}

	return 0;
}

int initialize_socket(t_malcolm *malcolm)
{
	struct ifaddrs *ifaddr, *ifa;

	malcolm->sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (malcolm->sockfd == -1)
	{
		return (handle_error("socket"));
	}

	if (set_socket_timeout(malcolm->sockfd, 1, 0) < 0)
	{
		return (handle_error("setsockopt"));
	}

	if (getifaddrs(&ifaddr) == -1)
	{
		close(malcolm->sockfd);
		return (handle_error("getifaddrs"));
	}

	if (malcolm->verbose)
	{
		printf("socket initialised.\n");
		printf("now looking for an available interface...\n");
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;

		if (ifa->ifa_flags & IFF_UP &&
			!(ifa->ifa_flags & IFF_LOOPBACK) &&
			ifa->ifa_flags & IFF_RUNNING &&
			ifa->ifa_addr->sa_family == AF_INET)
		{
			if (malcolm->verbose)
			{
				printf("interface found: %s\n", ifa->ifa_name);
			}
			break;
		}
	}

	if (ifa == NULL)
	{
		freeifaddrs(ifaddr);
		close(malcolm->sockfd);
		return (handle_error("no available interface"));
	}

	ft_memset(&malcolm->sll, 0, sizeof(malcolm->sll));
	malcolm->sll.sll_family = AF_PACKET;
	malcolm->sll.sll_ifindex = if_nametoindex(ifa->ifa_name);
	malcolm->sll.sll_protocol = htons(ETH_P_ARP);

	if (bind(malcolm->sockfd, (struct sockaddr *)&malcolm->sll, sizeof(malcolm->sll)) == -1)
	{
		freeifaddrs(ifaddr);
		close(malcolm->sockfd);
		return (handle_error("bind"));
	}

	printf(RED "listening on interface %s...\n" RESET, ifa->ifa_name);

	freeifaddrs(ifaddr);

	return 0;
}
