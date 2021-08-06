#include "../inc/ft_ping.h"

// static unsigned char checksum (unsigned char *ptr, size_t len) {
//     unsigned char chk = 0;
//     while (len--)
//         chk -= *ptr++;
//     return chk;
// }

static void build_ping_packet(struct ping_pkt pckt)
{
    static int msg_count = 0;
    unsigned int i;

    ft_bzero(&pckt, sizeof(pckt));
    pckt.hdr.type = ICMP_ECHO; // 8
    pckt.hdr.un.echo.id = getpid();
        
    for (i = 0; i < sizeof(pckt.msg)-1; i++ )
        pckt.msg[i] = i + '0';
        
    pckt.msg[i] = 0;
    pckt.hdr.un.echo.sequence = msg_count++;
    //pckt.hdr.checksum = checksum((unsigned char*)&pckt, sizeof(pckt));

}

int ft_ping()
{
    struct timeval t0;
    struct timeval t = {0,0};
    struct sockaddr_in r_addr;
    ssize_t err;
    struct ping_pkt pckt;
    socklen_t addr_len;

    gettimeofday(&t0, NULL);
    build_ping_packet(pckt);
    err = sendto(g_state.sockfd, (void*)&pckt, sizeof(pckt), 0, (struct sockaddr *)g_state.addr_list, sizeof(*g_state.addr_list));
    if (err == -1)
    {
        printf("%s: error: sendto failed\n", BIN);
        return (-1);
    }
    // usleep(100000);
    usleep(g_state.ttl * 1000);
    addr_len = sizeof(r_addr);
    (void)addr_len;
    err = recvfrom(g_state.sockfd, &pckt, 0, sizeof(pckt), (struct sockaddr*)&r_addr, &addr_len);
    //err = recvfrom(g_state.sockfd, (void*)&pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &addr_len);
    if (err == -1)
    {
        printf("%s: error: recvfrom failed\n", BIN);
        return (-1);
    }
    gettimeofday(&t, NULL);
    printf("time=%.2f ms\n", elapsed(t,t0));

	return(0);
}