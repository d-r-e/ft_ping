#include "../inc/ft_ping.h"

float elapsed(struct timeval a, struct timeval b)
{
    return (((float)(a.tv_usec - b.tv_usec) / 1000) +
        ((float)(a.tv_sec - b.tv_sec)) * 1000);
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
    while (g_state.loop && ms1 - ms0 < sec * 1000){
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
    while (g_state.loop && ms1 - ms0 < usec){
        gettimeofday(&t1, NULL);
        ms1 = t1.tv_sec * 1000000 + t1.tv_usec;
    }
}
