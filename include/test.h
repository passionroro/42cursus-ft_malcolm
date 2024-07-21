#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "ft_malcom.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define TEST(condition) do { \
    if (condition) { \
        printf(GREEN "PASSED" RESET "\n"); \
    } else { \
        printf(RED "FAILED" RESET "\n"); \
        exit(1); \
    } \
} while (0)

typedef struct s_test
{
    int     fd;
	char    **expect;
}	t_test;

char **test_invalid_ip();
char **test_invalid_mac();

