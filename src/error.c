#include "error.h"

int	handle_error(const char *message)
{
	fprintf(stderr, "Error: %s: %s\n", message, strerror(errno));
	return -1;
}
