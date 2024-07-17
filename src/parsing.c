#include "ft_malcom.h"

int	parse_ip_number(char *str)
{
	int ip_number, i = -1;

	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
		{
			errno = EINVAL;
			return (handle_error("non-digit character in ip"));
		}
	}

	ip_number = ft_atoi(str);
	if (ip_number < 0 || ip_number > 255)
	{
		errno = EINVAL;
		return (handle_error("ip must be between [0-255]"));
	}

	return 0;
}

int	parse_ip_address(char* str, const char* ip)
{
	char** ip_split = ft_split(str, '.');
	if (!ip_split)
	{
		errno = ENOMEM;
		return (handle_error("ft_split"));
	}

	int i = -1;
	while (ip_split[++i])
	{
		if (i >= 4)
			break ;
		if (parse_ip_number(ip_split[i]) != 0)
		{
			free_array(ip_split);
			return -1;
		}
	}

	if (ip_split[i] || i != 4)
	{
		errno = EINVAL;
		free_array(ip_split);
		return (handle_error("ip must be in IPv4 format: [x.x.x.x]"));
	}

	ip = str;

	return 0;
}

int	parse_arguments(char** argv, t_client *source, t_client *target)
{
	if (parse_ip_address(argv[1], source->ip) != 0 ||
		parse_ip_address(argv[3], target->ip) != 0)
	{
		return -1;
	}
	return 0;
}
