#include <ft_ping.h>

static volatile int keep_running = 1;

static void sigint_handler(int sig)
{
    (void)sig;
    keep_running = 0;
}

static int checksum(void *b, int len)
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

static void sig_handler(int signo)
{
    if (signo == SIGALRM)
        alarm(1);

}

static const char *get_ip(const char *hostname)
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

static void fill_icmp_packet(ping_pkt_t *pkt, unsigned int seq_number)
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

static int send_ping(int sockfd, struct sockaddr_in *addr, ping_pkt_t *pkt)
{
    unsigned int sequence = ntohs(pkt->hdr.un.echo.sequence);
    sequence++;
    pkt->hdr.un.echo.sequence = htons(sequence - 1);
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

static int set_ttl(int sockfd, unsigned int ttl)
{
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
    {
        perror("Setsockopt failed to set IP_TTL");
        return -1;
    }
    return 0;
}

static int setup_socket(unsigned int ttl)
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

static struct sockaddr_in prepare_dest_addr(const char *ip)
{
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &dest_addr.sin_addr);
    return dest_addr;
}


/// @brief Main function for ft_ping
/// @param hostname 
/// @param ttl 
/// @param count 
/// @param verbose 
/// @return 
int ft_ping(const char *hostname, unsigned int ttl, long count, int verbose)
{
    struct timeval timeout = {1, 0};
    unsigned int seq_number = 0;
    unsigned int total_sent = 0;
    unsigned int total_received = 0;
    ping_stats_t stats = {INFINITY, 0, 0, 0, 0};

    struct sigaction act;
    int sockfd;

    (void)verbose;
    const char *ip = get_ip(hostname);
    if (!ip)
    {
        fprintf(stderr, "Failed to resolve IP for hostname: %s\n", hostname);
        return 1;
    }
    if ((sockfd = setup_socket(ttl)) < 0)
        return 1;

    printf("PING %s (%s) %ld data bytes\n", hostname, ip, PING_PKT_SIZE - sizeof(struct icmphdr));
    struct sockaddr_in dest_addr = prepare_dest_addr(ip);

    memset(&act, 0, sizeof(act));
    if (count != 1)
    {
        struct sigaction act_int;
        memset(&act_int, 0, sizeof(act_int));
        act_int.sa_handler = sigint_handler;

        if (sigaction(SIGINT, &act_int, NULL) == -1)
        {
            perror("Error setting SIGINT handler");
            close(sockfd);
            return 1;
        }
        act.sa_handler = &sig_handler;
        sigaction(SIGALRM, &act, NULL);
        alarm(1);
    }

    while (count--)
    {
        if (!keep_running)
            break;
        ping_pkt_t pkt;
        fill_icmp_packet(&pkt, seq_number++);
        gettimeofday(&pkt.timestamp, NULL);
        if (send_ping(sockfd, &dest_addr, &pkt) == -1)
        {
            close(sockfd);
            return 1;
        } else {
            total_sent++;
        }
        int reply_status = handle_reply(sockfd, verbose, &pkt, &dest_addr, &timeout, &stats);
        if (reply_status == 1)
        {
            total_received++;
        }

        if (count == INT_MIN)
            count = -1;
        if (count != 0)
            pause();
    }
    print_stats(hostname, total_sent, total_received, &stats);
    total_sent = 0;
    total_received = 0;
    close(sockfd);
    return total_received == total_sent ? 0 : 1;
}
