#include "ft_malcolm.h"

// MAC
static int parse_mac_octet(const char *str)
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

static int parse_mac_address(const char *str, uint8_t (*mac)[ETH_ALEN])
{
	char **mac_split = ft_split(str, ':');
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
		(*mac)[i] = (uint8_t)strtol(mac_split[i], NULL, 16);
	}

	free_array(mac_split);

	return 0;
}

// IP
static int parse_ip_octet(const char *str)
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

static int parse_ip_address(const char *str, uint8_t (*ip)[IPV4_LENGTH])
{
	char **ip_split = ft_split(str, '.');
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

	int ip_conversion = inet_pton(AF_INET, str, *ip);
	if (ip_conversion != 1)
	{
		free_array(ip_split);
		if (ip_conversion == 0)
		{
			errno = EINVAL;
		}
		else if (ip_conversion == -1)
		{
			errno = EAFNOSUPPORT;
		}
		handle_error("ip conversion text to binary failed");
	}

	free_array(ip_split);

	return 0;
}

// VERBOSE
static int parse_verbose_mode(char *arg, bool *verbose)
{
	if (ft_strncmp(arg, "-v\0", 3))
	{
		errno = EINVAL;
		handle_error("did you meant to activate the verbose mode ?");
		print_help("./ft_malcolm");
		return -1;
	}
	*verbose = true;
	printf("setting verbose mode to true : %d\n", *verbose);
	return 0;
}

// MAIN
int parse_arguments(char **argv, t_client *source, t_client *target, bool *verbose)
{
	if (parse_ip_address(argv[1], &(source->ip)) != 0 ||
		parse_ip_address(argv[3], &(target->ip)) != 0)
	{
		return -1;
	}

	if (parse_mac_address(argv[2], &(source->mac)) != 0 ||
		parse_mac_address(argv[4], &(target->mac)) != 0)
	{
		return -1;
	}

	if (argv[5] && parse_verbose_mode(argv[5], verbose) != 0)
	{
		return -1;
	}

	if (*verbose == true)
	{
		printf(GREEN "parsing successfull!\n");
		printf("source:\n- ip: ");
		print_ip(source->ip);
		printf("- mac: ");
		print_mac(source->mac);
		printf("target:\n- ip: ");
		print_ip(target->ip);
		printf("- mac: ");
		print_mac(target->mac);
		printf("\n" RESET);
	}

	return 0;
}
