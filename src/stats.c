#include "../inc/ft_ping.h"

double ft_pow(double base, double exp)
{
    long double n;

    if (exp < 0)
        return (0);
    if (exp == 0)
        return (1);
    n = base;
    while (--exp){
        n = n * base;
    }
    return (n);
}

double ft_mean(double data[], int n)
{
    double sum;
    
    if (!data || n <= 0)
        return (0);
    sum = 0;
    for (int i = 0; i < n; ++i)
        sum += data[i];
    return (sum / n);
}

double ft_variance(double data[], int n)
{
    double std;
    double mean;

    if (!data || n <= 0)
        return (0);
    mean = ft_mean(data, n);
    std = 0;
    for (int i = 0; i < n; ++i)
        std += ft_pow(data[i] - mean, 2);
    return (std / n);
}

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
    if (g_state.p_transmitted > 0)
        printf("round-trip min/avg/max = %.3f/%.3f/%.3f ms\n", g_state.min, g_state.avg, g_state.max);
}