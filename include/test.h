#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ft_malcom.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define MAX_ERROR_LENGTH 256
#define MAX_ERRORS 6

#define TEST(name, condition) do { \
    printf("%s... ", name); \
    if (condition) { \
        printf(GREEN "PASSED" RESET "\n"); \
    } else { \
        printf(RED "FAILED" RESET "\n"); \
        exit(1); \
    } \
} while (0)

