#include "../inc/ft_ping.h"

int ft_ping()
{
    struct timeval t0;
    struct timeval t = {0,0};
    static int msg_count = 0;
    // struct ping_pkt pak;

    msg_count++;
    gettimeofday(&t0, NULL);
    sleep(1);
    gettimeofday(&t, NULL);
    printf("%.2fms\n", timestamp(t,t0));
	(void)t;
	return(0);
}