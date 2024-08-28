#include "ft_malcolm.h"

int initialize_socket(t_malcolm *malcolm)
{
	struct ifaddrs *ifaddr, *ifa;

	// check the socket ! ALL or ARP ? RAW or DGRAM ?
	malcolm->sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (malcolm->sockfd == -1) 
	{
		return (handle_error("socket"));
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

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
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
			ft_strlcpy(malcolm->if_name, ifa->ifa_name, IFNAMSIZ);
			break;
		}
	}

	if (ifa == NULL)
	{
		close(malcolm->sockfd);
		return (handle_error("no available interface"));
	}

	memset(&malcolm->sll, 0, sizeof(malcolm->sll));
	malcolm->sll.sll_family = AF_PACKET;
	malcolm->sll.sll_ifindex = if_nametoindex(malcolm->if_name);
	malcolm->sll.sll_protocol = htons(ETH_P_ALL);

	if (bind(malcolm->sockfd, (struct sockaddr*)&malcolm->sll, sizeof(malcolm->sll)) == -1) 
	{
		close(malcolm->sockfd);
		return (handle_error("bind"));
	}

	printf("listening on interface %s...\n", malcolm->if_name);

	return 0;
}
