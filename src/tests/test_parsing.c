#include "test.h"

char *expect_ip[6] = {
    "ip must be between [0-255]",
    "non-digit character in ip",
    "ip must contain 4 octets",
    "ip must contain 4 octets",
    "non-digit character in ip",
    "ip octet can not have more that 3 numbers"
};

char **test_invalid_ip() 
{
    parse_ip_address("256.0.0.1", NULL);
    parse_ip_address("0.0.0.-1", NULL);
    parse_ip_address("0.0.0", NULL);
    parse_ip_address("0.0.0.0.0", NULL);
    parse_ip_address("0.0.0.0x", NULL);
    parse_ip_address("0.0.0.0000", NULL);

    return expect_ip;
}


char *expect_mac[5] = {
    "mac octet must be hexadecimal",
    "mac octet must have a length of 2",
    "mac must contain 6 octets",
    "mac must contain 6 octets",
    "mac octet must have a length of 2"
};

char **test_invalid_mac() 
{
    parse_mac_address("00:aa:ff:99:00:fg", NULL);
    parse_mac_address("00:aa:ff:99:00:-ee", NULL);
    parse_mac_address("00:aa:ff:99:00", NULL);
    parse_mac_address("00:aa:ff:99:00:ee:00", NULL);
    parse_mac_address("00:aa:ff:99:00:eee", NULL);
    
    return expect_mac;
}