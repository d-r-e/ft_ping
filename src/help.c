#include <ft_ping.h>


void usage(void)
{
    printf("Usage: ./ft_ping <hostname>\n");
    exit(EXIT_FAILURE);
}

void version(void)
{
    printf("ft_ping version 1.0 by darodrig\n");
    exit(EXIT_SUCCESS);
}

void help(void)
{
    printf("ft_ping by darodrig at 42 Madrid (2024)\n");
    printf("Usage: ./ft_ping <hostname>\n");
    printf("Options:\n");
    printf("  -?, --help     Display this information\n");
    printf("  -V, --version  Display version information\n");
    printf("  -v, --verbose  Verbose output\n");
    printf("  -c, --count    Number of packets to send\n");
    printf("  -w, --timeout  Timeout in milliseconds\n");
    printf("  --ttl          Time to live\n");

}