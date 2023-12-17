#include "ft_ping.h"

void print_version(void)
{
    printf("%s: Version 1.0 by darodrig\n", BIN);
    exit(0);
}

void print_error(char *message)
{
    printf("%s: %s\n", BIN, message);
    printf("Try '%s --help' or '%s --usage' for more information.\n", BIN, BIN);
    exit(1);
}

void print_help(void)
{
    printf("Usage: %s [OPTION]... HOST\n", BIN);
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");

    printf("\n");
    printf("Options valid for all request types:\n");
    printf("  -h, --help            display this help and exit\n");
    printf("  -V, --version         output version information and exit\n");
    printf("  -c, --count=NUMBER    stop after sending NUMBER packets\n");
}
