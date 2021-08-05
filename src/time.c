#include "../inc/ft_ping.h"

float elapsed(struct timeval a, struct timeval b)
{
    return (((float)(a.tv_usec - b.tv_usec) / 1000) +
        ((float)(a.tv_sec - b.tv_sec)) * 1000);
}