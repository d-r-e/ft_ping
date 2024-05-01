#include <ft_ping.h>

int handle_reply(int sockfd, struct sockaddr_in *dest_addr, struct timeval *timeout)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    int ret = select(sockfd + 1, &readfds, NULL, NULL, timeout);
    if (ret == -1)
    {
        perror("Select failed");
        return -1;
    }
    else if (ret)
    {
        uint8_t buffer[1024];
        ssize_t rd = read(sockfd, buffer, sizeof(buffer));
        if (rd > 0)
        {
            struct iphdr *ip_reply = (struct iphdr *)buffer;
            struct icmphdr *icmp_reply = (struct icmphdr *)(buffer + (ip_reply->ihl << 2));
            unsigned int reply_ttl = ip_reply->ttl;
            print_icmp_reply(icmp_reply, dest_addr, rd - (ip_reply->ihl << 2), reply_ttl);
        }
        return 1;
    }
    return 0;
}

void print_icmp_reply(struct icmphdr *icmp_reply, struct sockaddr_in *addr, size_t read_bytes, size_t reply_ttl)
{
    printf("%ld bytes from %s: icmp_seq=%d ttl=%ld time=%.2f ms\n",
              read_bytes,
           inet_ntoa(addr->sin_addr),
           ntohs(icmp_reply->un.echo.sequence),
              reply_ttl,
           0.0);
}