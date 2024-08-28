#include "ft_malcolm.h"

int run(t_malcolm *malcolm)
{
	if (initialize_socket(malcolm) < 0)
	{
		return -1;
	}

	int result = read_and_process_packets(malcolm);

	close(malcolm->sockfd);
	printf("good-bye!\n");

	return result;
}

int main(int argc, char **argv)
{
	t_client source, target;
	t_malcolm malcolm = {0};

	if (argc < 5 || argc > 6)
	{
		print_help(argv[0]);
		return -1;
	}

	if (parse_arguments(argv, &source, &target, &malcolm.verbose) != 0)
	{
		return -1;
	}

	malcolm.source = source;
	malcolm.target = target;

	return run(&malcolm);
}
