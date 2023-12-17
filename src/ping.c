#include "ft_ping.h"

int socket_init(t_state state)
{
    char *host = state.options.host;
    struct addrinfo hints;
    struct addrinfo *res;
    // struct addrinfo *ressave;
    int n;

    ft_bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    if ((n = getaddrinfo(host, NULL, &hints, &res)) != 0)
        print_error("getaddrinfo error for host");
    // ressave = res;
    state.addr_list = res;
    state.protocol = res->ai_protocol;
    if ((state.sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
        print_error("socket error");
    if (state.options.ttl_set)
    {
        if (setsockopt(state.sockfd, IPPROTO_IP, IP_TTL, &state.ttl, sizeof(state.ttl)) < 0)
            print_error("setsockopt error");
    }
    if (connect(state.sockfd, res->ai_addr, res->ai_addrlen) < 0)
        print_error("connect error");

    // freeaddrinfo(ressave);
    return (0);
}

void receive_reply(t_state state)
{
    ssize_t read;
    t_icmp_packet pckt;
    struct sockaddr_in r_addr;
    socklen_t len = sizeof(r_addr);
    struct timeval t1;

    ft_bzero(&pckt, sizeof(t_icmp_packet));
    read = recvfrom(state.sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr *)&r_addr, &len);
    if (read <= 0)
    {
        printf("%s: error: recvfrom failed\n", BIN);
        return;
    }
    gettimeofday(&t1, NULL);
    state.rx++;
    if (state.options.verbose)
    {
        printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%ld ms\n", \
                read, inet_ntoa(r_addr.sin_addr), \
                SWAP16(pckt.icmphdr.un.echo.sequence), \
                pckt.msg[8], \
                (t1.tv_usec - state.t0.tv_usec) / 1000);
    }
    else
    {
        printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%ld ms\n", \
                read, inet_ntoa(r_addr.sin_addr), \
                SWAP16(pckt.icmphdr.un.echo.sequence), \
                pckt.msg[8], \
                (t1.tv_usec - state.t0.tv_usec) / 1000);
    }
}


void ft_ping(t_state state)
{
    ssize_t read;
    t_icmp_packet pckt;

    ft_bzero(&pckt, sizeof(t_icmp_packet));

    gettimeofday(&state.t0, NULL);
    build_ping_packet(&pckt, state.t0);
    read = sendto(state.sockfd, \
                (void *)&pckt, sizeof(pckt), 0, \
                (struct sockaddr *)((state.addr_list)->ai_addr), \
                (state.addr_list)->ai_addrlen);
    if (read <= 0)
    {
        printf("%s: error: sendto failed\n", BIN);
        return;
    }
    state.tx++;
    if (state.options.flood)
    {
        printf(".");
    }
    if (state.options.flood == 0)
        alarm(1);
    printf("DEBUG: ping sent\n");
    receive_reply(state);
}