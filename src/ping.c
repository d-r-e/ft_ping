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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return NULL;
    }

    const char *ret_ip = NULL;
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
        ret_ip = ipstr;
        break;
    }

    freeaddrinfo(res);
    return ret_ip;
}

void fill_icmp_packet(ping_pkt_t *pkt, unsigned int seq_number)
{
    memset(pkt, 0, PING_PKT_SIZE);
    pkt->hdr.type = ICMP_ECHO;
    pkt->hdr.un.echo.id = getpid() & 0xFFFF;

    pkt->hdr.un.echo.sequence = htons(seq_number);

    for (long unsigned int i = 0; i < sizeof(pkt->msg) - 1; i++)
    {
        pkt->msg[i] = '0' + (char)(i % 10);
    }
    pkt->msg[sizeof(pkt->msg) - 1] = '\0';
}

int send_ping(int sockfd, struct sockaddr_in *addr, ping_pkt_t *pkt)
{
    unsigned int sequence = ntohs(pkt->hdr.un.echo.sequence);
    sequence++;
    pkt->hdr.un.echo.sequence = htons(sequence);
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

int set_ttl(int sockfd, unsigned int ttl)
{
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
    {
        perror("Setsockopt failed to set IP_TTL");
        return -1;
    }
    return 0;
}

int setup_socket(unsigned int ttl)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return -1;
    }
    if (set_ttl(sockfd, ttl) < 0)
    {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

struct sockaddr_in prepare_dest_addr(const char *ip)
{
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &dest_addr.sin_addr);
    return dest_addr;
}

void total_timeout_handler(int signo)
{
    (void)signo;
    fprintf(stderr, "Total timeout elapsed\n");
    exit(1);
}

int setup_total_timeout(unsigned int timeout_seconds)
{
    struct itimerval timer;
    timer.it_value.tv_sec = timeout_seconds;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    {
        perror("Failed to set total timeout");
        return -1;
    }
    return 0;
}

int ft_ping(const char *hostname, unsigned int ttl, long count, unsigned int timeout_seconds, int verbose)
{
    struct timeval timeout = {timeout_seconds, 0};
    unsigned int seq_number = 0;
    unsigned int received = 0;
    struct sigaction act, total_timeout_act;
    int sockfd;

    (void)verbose;
    const char *ip = get_ip(hostname);
    if (!ip)
    {
        fprintf(stderr, "Failed to resolve IP for hostname: %s\n", hostname);
        return 1;
    }

    sockfd = setup_socket(ttl);
    if (sockfd < 0)
        return 1;

    printf("PING %s (%s) %ld data bytes\n", hostname, ip, PING_PKT_SIZE - sizeof(struct icmphdr));
    struct sockaddr_in dest_addr = prepare_dest_addr(ip);

    memset(&act, 0, sizeof(act));
    if (count != 1)
    {
        act.sa_handler = &sig_handler;
        sigaction(SIGALRM, &act, NULL);
        alarm(1);
    }

    memset(&total_timeout_act, 0, sizeof(total_timeout_act));
    total_timeout_act.sa_handler = total_timeout_handler;
    sigaction(SIGALRM, &total_timeout_act, NULL);

    if (setup_total_timeout(timeout_seconds) < 0)
    {
        close(sockfd);
        return 1;
    }

    while (count--)
    {
        ping_pkt_t pkt;
        fill_icmp_packet(&pkt, seq_number++);
        if (send_ping(sockfd, &dest_addr, &pkt) == -1)
        {
            close(sockfd);
            return 1;
        }

        int reply_status = handle_reply(sockfd, &dest_addr, &timeout);
        if (reply_status == 1)
        {
            received++;
        }
        else if (reply_status == 0)
        {
            printf("Request timeout for icmp_seq %d\n", seq_number);
        }
        if (count == INT_MIN)
            count = -1;
        if (count != 0)
            pause();
    }

    close(sockfd);
    return 0;
}
