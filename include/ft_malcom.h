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
int	parse_ip_number(char *str);
int	parse_ip_address(char* str, const char* ip);
int	parse_arguments(char** argv, t_client *source, t_client *target);
