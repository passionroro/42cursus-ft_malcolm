#include "test.h"

t_test redirect_stderr_to_file(const char* test_name, char **(*test_function)()) 
{
    t_test  test;
    char    filename[64];

    snprintf(filename, sizeof(filename), "stderr_%s.txt", test_name);
    
    freopen(filename, "w", stderr);
    
    test.expect = test_function();
    
    fflush(stderr);
    freopen("/dev/tty", "w", stderr);
    
    test.fd = open(filename, O_RDONLY);
    
    remove(filename);
    return test;
}

void    run_test(const char* test_name, char **(*test_function)())
{
    t_test    test;
	int nb = 0;
	char	*test_line;

    printf("Testing %s...\n", test_name);

	test = redirect_stderr_to_file(test_name, test_function);
	test_line = get_next_line(test.fd);
	while (test_line)
	{
        TEST(strstr(test_line, test.expect[nb]) != NULL);
		free(test_line);
		test_line = get_next_line(test.fd);
        nb++;
	}
	close (test.fd);

    printf(GREEN "%s passed successfully!" RESET "\n\n", test_name);
}

int main()
{
    run_test("invalid_ip", test_invalid_ip);
    run_test("invalid_mac", test_invalid_mac);
    return 0;
}
