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
    //printf("%.56s\n", pckt.msg);
    pckt.msg[i] = 0;
    pckt.hdr.un.echo.sequence = msg_count++;
    //pckt.hdr.checksum = checksum((unsigned char*)&pckt, sizeof(pckt));
}



int ft_ping()
{
    struct timeval t0 = {0,0};
    struct timeval t = {0,0};
    struct sockaddr_in r_addr;
    ssize_t err;
    struct ping_pkt pckt = {};
    socklen_t addr_len;

    ft_bzero(&pckt, sizeof(pckt));
    build_ping_packet(pckt);
    sleep(1);
    gettimeofday(&t0, NULL);
    err = sendto(g_state.sockfd, (void*)&pckt, sizeof(pckt), 0, (struct sockaddr *)g_state.addr_list, sizeof(*g_state.addr_list));
    if (err == -1)
    {
        printf("%s: error: sendto failed\n", BIN);
        return (-1);
    }
    g_state.p_transmitted++;
    // usleep(100000);
    addr_len = sizeof(r_addr);
    (void)addr_len;
    err = recvfrom(g_state.sockfd, &pckt, 0, sizeof(pckt), (struct sockaddr*)&r_addr, &addr_len);
    //err = recvfrom(g_state.sockfd, (void*)&pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &addr_len);
    gettimeofday(&t, NULL);
    g_state.p_received++;
    printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n", \
        PING_SZ, g_state.host, g_state.p_transmitted - 1, g_state.ttl, elapsed(t,t0));
    if (err == -1)
    {
        printf("%s: error: recvfrom failed\n", BIN);
        return (-1);
    }

	return(0);
}