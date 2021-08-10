#include "../inc/ft_ping.h"

float elapsed(struct timeval a, struct timeval b)
{
    double t0 = a.tv_usec + a.tv_sec * 1000000;
    double t1 = b.tv_usec + b.tv_sec * 1000000;
    //printf("%f\n", t1 - t0);
    double time = (t0 - t1) / 1000;
    g_state.sum += time;
    g_state.avg = g_state.sum / g_state.p_received;
    g_state.msum += ABS(g_state.avg - time);
    g_state.mdev = g_state.msum / g_state.p_received;
    g_state.min = g_state.min != 0 && time > g_state.min ? g_state.min : time;
    g_state.max = time >= g_state.max ? time : g_state.max;
    return (time);
}

void ft_sleep(long long sec)
{
    struct timeval t0;
    struct timeval t1;
    long long ms0;
    long long ms1;

    if (sec <= 0)
        return;
    gettimeofday(&t0, NULL);
    gettimeofday(&t1, NULL);
    ms0 = t0.tv_sec * 1000 + t0.tv_usec / 1000;
    ms1 = t1.tv_sec * 1000 + t1.tv_usec / 1000;
    while (g_state.loop && ms1 - ms0 < sec * 1000)
    {
        gettimeofday(&t1, NULL);
        ms1 = t1.tv_sec * 1000 + t1.tv_usec / 1000;
    }
}

void ft_usleep(long long usec)
{
    struct timeval t0;
    struct timeval t1;
    long long ms0;
    long long ms1;

    if (usec <= 0)
        return;
    gettimeofday(&t0, NULL);
    gettimeofday(&t1, NULL);
    ms0 = t0.tv_sec * 1000000 + t0.tv_usec;
    ms1 = t1.tv_sec * 1000000 + t1.tv_usec;
    while (g_state.loop && ms1 - ms0 < usec)
    {
        gettimeofday(&t1, NULL);
        ms1 = t1.tv_sec * 1000000 + t1.tv_usec;
    }
}
