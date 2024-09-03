#include "ft_malcolm.h"

int signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("Ctrl+C intercepted.\n");
        return -1;
    }
    return 1;
}