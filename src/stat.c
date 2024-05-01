#include <ft_ping.h>


void print_stats(const char *hostname, unsigned int total_sent, unsigned int total_received)
{
    printf("--- %s ping statistics ---\n", hostname);
    printf("%u packets transmitted, %u received, %.1f%% packet loss\n",
           total_sent, total_received,
           ((total_sent - total_received) / (double)total_sent) * 100.0);
}