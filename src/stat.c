#include <ft_ping.h>


void print_stats()
{
    printf("\n--- Ping statistics ---\n");
    printf("%u packets transmitted, %u received, %.1f%% packet loss\n",
           total_sent, total_received,
           ((total_sent - total_received) / (double)total_sent) * 100.0);
}