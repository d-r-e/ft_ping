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
        { // Check for IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        else
        { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        // Convert the IP to a string and return
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        freeaddrinfo(res); // Free the linked list
        return ipstr;
    }

    freeaddrinfo(res); // Free the linked list
    return NULL;       // Failed to resolve
}

int ft_ping(const char *hostname, unsigned int ttl, long count, unsigned int timeout)
{
    (void)ttl;
    (void)timeout;
    const char *ip = get_ip(hostname);
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = &sig_handler;
    sigaction(SIGALRM, &act, NULL);
    alarm(1);

    printf("PING %s (%s) 56 data bytes\n", hostname, ip);
    while (count--)
    {
        pause();
    }
    return 0;
}