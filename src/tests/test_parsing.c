#include "test.h"

char *expect_ip[6] = {
    "ip must be between [0-255]",
    "non-digit character in ip",
    "ip must contain 4 octets",
    "ip must contain 4 octets",
    "non-digit character in ip",
    "ip octet can not have more that 3 numbers"};

char **test_invalid_ip()
{
    uint8_t test_ip[IPV4_LENGTH];

    parse_ip_address("256.0.0.1", &test_ip);
    parse_ip_address("0.0.0.-1", &test_ip);
    parse_ip_address("0.0.0", &test_ip);
    parse_ip_address("0.0.0.0.0", &test_ip);
    parse_ip_address("0.0.0.0x", &test_ip);
    parse_ip_address("0.0.0.0000", &test_ip);

    return expect_ip;
}

char *expect_mac[6] = {
    "mac octet must be hexadecimal",
    "mac octet must be hexadecimal",
    "mac octet must have a length of 2",
    "mac must contain 6 octets",
    "mac must contain 6 octets",
    "mac octet must have a length of 2"};

char **test_invalid_mac()
{
    uint8_t test_mac[ETH_ALEN];

    parse_mac_address("00:aa:ff:99:00:fg", &test_mac);
    parse_mac_address("00:aa:ff:99:00:-e", &test_mac);
    parse_mac_address("00:aa:ff:99:00:-ee", &test_mac);
    parse_mac_address("00:aa:ff:99:00", &test_mac);
    parse_mac_address("00:aa:ff:99:00:ee:00", &test_mac);
    parse_mac_address("00:aa:ff:99:00:eee", &test_mac);

    return expect_mac;
}