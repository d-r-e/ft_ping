#include "../inc/ft_ping.h"

void print_stats()
{
    double percent;

    if (g_state.p_transmitted == 0)
        percent = 0;
    else
        percent = 100.0 * (g_state.p_transmitted - g_state.p_received) / g_state.p_transmitted;
    printf("--- %s ft_ping statistics ---\n", g_state.hostname);
    printf("%d packets transmitted, %d packets received, %0.0f%% packet loss\n", \
        g_state.p_transmitted, g_state.p_received, \
        percent);
}