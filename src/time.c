#include "../inc/ft_ping.h"

float elapsed(struct timeval a, struct timeval b)
{
    return (((float)(a.tv_usec - b.tv_usec) / 1000) +
        ((float)(a.tv_sec - b.tv_sec)) * 1000);
}

void ft_sleep(unsigned long long sec)
{
    struct timeval t0;
    struct timeval t1;

    if (sec <= 0)
        return;
    gettimeofday(&t0, NULL);
    gettimeofday(&t1, NULL);
    while ((unsigned long long)(t1.tv_sec - t0.tv_sec) < sec)
        ;
}

void ft_usleep(unsigned long long usec)
{
    struct timeval t0;
    struct timeval t1;

    if (usec <= 0)
        return;
    ft_sleep(usec / 1000000000);
    gettimeofday(&t0, NULL);
    gettimeofday(&t1, NULL);
    while ((unsigned long long)(t1.tv_usec - t0.tv_usec) < usec)
        ;
}
