#include "ft_malcolm.h"

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        errno = EINTR;
        g_ok = -1;
    }
}