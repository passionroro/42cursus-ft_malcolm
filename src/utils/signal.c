#include "ft_malcolm.h"

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("Ctrl+C intercepted.\n");
    }
}