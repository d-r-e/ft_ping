#include <ft_ping.h>

int checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void sig_handler(int signo)
{
    if (signo == SIGALRM)
        alarm(1);
}

const char *get_ip(const char *hostname)
{
    static char ipstr[INET6_ADDRSTRLEN];
    struct addrinfo hints, *res, *p;
    int status;
    void *addr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // AF_INET for IPv4 only, AF_INET6 for IPv6, AF_UNSPEC for any
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return NULL;
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        else
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        freeaddrinfo(res);
        return ipstr;
    }

    freeaddrinfo(res);
    return NULL;
}

void fill_icmp_packet(ping_pkt_t *pkt)
{
    memset(pkt, 0, PING_PKT_SIZE);
    pkt->hdr.type = ICMP_ECHO;
    pkt->hdr.un.echo.id = getpid() & 0xFFFF;
    for (long unsigned int i = 0; i < sizeof(pkt->msg) - 1; i++)
        pkt->msg[i] = i + '0';
    pkt->msg[sizeof(pkt->msg) - 1] = 0;
    pkt->hdr.checksum = checksum(pkt, PING_PKT_SIZE);
}

int send_ping(int sockfd, struct sockaddr_in *addr, ping_pkt_t *pkt)
{
    pkt->hdr.un.echo.sequence++;
    pkt->hdr.checksum = 0;
    pkt->hdr.checksum = checksum(pkt, PING_PKT_SIZE);

    int sent = sendto(sockfd, pkt, PING_PKT_SIZE, 0, (struct sockaddr *)addr, sizeof(*addr));
    if (sent <= 0)
    {
        perror("Failed to send ping");
        return -1;
    }

    return 0;
}

int ft_ping(const char *hostname, unsigned int ttl, long count, unsigned int timeout_ms)
{
    (void)ttl;
    struct timeval timeout = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};

    const char *ip = get_ip(hostname);
    if (!ip)
    {
        fprintf(stderr, "Failed to resolve IP for hostname: %s\n", hostname);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = &sig_handler;
    sigaction(SIGALRM, &act, NULL);
    alarm(1);

    printf("PING %s (%s) %d data bytes\n", hostname, ip, PING_PKT_SIZE);
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &dest_addr.sin_addr);

    while (count--)
    {
        ping_pkt_t pkt;
        fill_icmp_packet(&pkt);
        if (send_ping(sockfd, &dest_addr, &pkt) == -1)
        {
            close(sockfd);
            return 1;
        }

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        int ret = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        if (ret == -1)
        {
            perror("Select failed");
            close(sockfd);
            return 1;
        }
        else if (ret)
        {
            printf("Received packet from %s\n", ip);
        }
        pause();
    }

    close(sockfd);
    return 0;
}