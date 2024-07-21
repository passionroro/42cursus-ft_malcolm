#include "ft_malcom.h"

// MAC
int	parse_mac_octet(char *str)
{
	int i = -1;
	
	if (ft_strlen(str) != 2)
	{
		errno = EINVAL;
		return (handle_error("mac octet must have a length of 2"));
	}

	while (str[++i])
	{
		if (!ft_ishexadecimal(str[i]))
		{
			errno = EINVAL;
			return (handle_error("mac octet must be hexadecimal"));
		}
	}

	return 0;
}

int	parse_mac_address(char *str, const char *mac)
{
	char** mac_split = ft_split(str, ':');
	if (!mac_split)
	{
		errno = ENOMEM;
		return (handle_error("ft_split"));
	}

	size_t length = get_array_length(mac_split);
	if (length != 6)
	{
		errno = EINVAL;
		free_array(mac_split);
		return (handle_error("mac must contain 6 octets"));
	}

	int i = -1;
	while (mac_split[++i])
	{
		if (parse_mac_octet(mac_split[i]) != 0)
		{
			free_array(mac_split);
			return -1;
		}
	}

	mac = str;
	(void)mac;

	return 0;
}

// IP
int	parse_ip_octet(char *str)
{
	int octet, i = -1;
	
	if (ft_strlen(str) > 3)
	{
		errno = EINVAL;
		return (handle_error("ip octet can not have more that 3 numbers"));
	}

	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
		{
			errno = EINVAL;
			return (handle_error("non-digit character in ip"));
		}
	}

	octet = ft_atoi(str);
	if (octet < 0 || octet > 255)
	{
		errno = EINVAL;
		return (handle_error("ip must be between [0-255]"));
	}

	return 0;
}

int	parse_ip_address(char *str, const char *ip)
{
	char** ip_split = ft_split(str, '.');
	if (!ip_split)
	{
		errno = ENOMEM;
		return (handle_error("ft_split"));
	}

	size_t length = get_array_length(ip_split);
	if (length != 4)
	{
		errno = EINVAL;
		free_array(ip_split);
		return (handle_error("ip must contain 4 octets"));
	}

	int i = -1;
	while (ip_split[++i])
	{
		if (parse_ip_octet(ip_split[i]) != 0)
		{
			free_array(ip_split);
			return -1;
		}
	}

	ip = str;
	(void)ip;

	return 0;
}

// MAIN
int	parse_arguments(char** argv, t_client *source, t_client *target)
{
	if (parse_ip_address(argv[1], source->ip) != 0 ||
		parse_ip_address(argv[3], target->ip) != 0)
	{
		return -1;
	}

	if (parse_mac_address(argv[2], source->mac) != 0 ||
		parse_mac_address(argv[4], target->mac) != 0)
	{
		return -1;
	}

	return 0;
}
