#pragma once

#include "../libft/libft.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct s_client
{
	const char*	ip;
	const char*	mac;
}	t_client;

// parsing.c
int	parse_ip_octet(char *str);
int	parse_ip_address(char *str, const char *ip);
int	parse_mac_octet(char *str);
int	parse_mac_address(char *str, const char *mac);
int	parse_arguments(char **argv, t_client *source, t_client *target);

// utils.c
int		ft_ishexadecimal(int c);
size_t	get_array_length(char **arr);
