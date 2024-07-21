#include "ft_malcom.h"

// parsing.c
int ft_ishexadecimal(int c)
{
	return (ft_isdigit(c) || (c >= 'a' && c <= 'f'));
}

size_t	get_array_length(char **arr)
{
	size_t i = -1;
	while (arr[++i]) { ; }
	return i;
}
