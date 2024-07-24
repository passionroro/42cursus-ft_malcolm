#include "ft_malcolm.h"

void	run(void)
{
}

int	main(int argc, char **argv)
{
	t_arp_header	arp;

	if (argc != 5)
	{
		printf("Usage: %s [source ip] [source mac address] [target ip] [target mac address]\n", argv[0]);
		return -1;
	}

	if (parse_arguments(argv, &arp) != 0)
	{
		return -1;
	}

	arp.hardware_type = htons(HARDWARE);
	arp.protocol_type = htons(PROTOCOL);
	arp.hardware_len = MAC_LENGTH;
	arp.protocol_len = IPV4_LENGTH;
	arp.opcode = htons(REQUEST);

	verbose(arp);

	run();

	return 0;
}

