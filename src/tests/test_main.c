#include "test.h"

void capture_stderr(const char* test_name, void (*test_function)(), char errors[MAX_ERRORS][MAX_ERROR_LENGTH]) {
    char filename[64];
    snprintf(filename, sizeof(filename), "stderr_%s.txt", test_name);
    
    // Redirect stderr to a file
    freopen(filename, "w", stderr);
    
    // Run the test
    test_function();
    
    // Restore stderr
    fflush(stderr);
    freopen("/dev/tty", "w", stderr);
    
    // Read the captured output
    FILE *f = fopen(filename, "r");
    if (f) {
        for (int i = 0; i < MAX_ERRORS; i++) {
            if (fgets(errors[i], MAX_ERROR_LENGTH, f) == NULL) {
                break;
            }
            // Remove newline character if present
            errors[i][strcspn(errors[i], "\n")] = 0;
        }
        fclose(f);
    }
    
    // Remove the temporary file
    remove(filename);
}

void test_invalid_ip() {
    parse_ip_address("256.0.0.1", NULL);
    parse_ip_address("0.0.0.-1", NULL);
    parse_ip_address("0.0.0", NULL);
    parse_ip_address("0.0.0.0.0", NULL);
    parse_ip_address("0.0.0.0x", NULL);
    parse_ip_address("0.0.0.0000", NULL);
}

int main() {
    char errors[MAX_ERRORS][MAX_ERROR_LENGTH] = {0};
    capture_stderr("invalid_ip", test_invalid_ip, errors);
    
    TEST("IP > 255", strstr(errors[0], "ip must be between [0-255]") != NULL);
    TEST("Negative IP", strstr(errors[1], "non-digit character in ip") != NULL);
    TEST("Incomplete IP", strstr(errors[2], "ip must be in IPv4 format: [x.x.x.x]") != NULL);
    TEST("Too many octets", strstr(errors[3], "ip must be in IPv4 format: [x.x.x.x]") != NULL);
    TEST("Non-digit character", strstr(errors[4], "non-digit character in ip") != NULL);
    TEST("Too many digits", strstr(errors[5], "ip must be between [0-255]") != NULL);

    printf(GREEN "All tests passed successfully!" RESET "\n");
    return 0;
}
