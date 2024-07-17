#include "ft_malcom.h"

int	main(int argc, char **argv)
{
	t_client	source;
	t_client	target;

	if (argc != 5)
	{
		errno = EINVAL;
		return (handle_error("Expected Arguments: [source ip] [source mac address] [target ip] [target mac address]"));
	}

	if (parse_arguments(argv, &source, &target) != 0)
	{
		return -1;
	}

	return 0;
}

