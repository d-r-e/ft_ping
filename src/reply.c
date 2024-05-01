#include <ft_ping.h>

int handle_reply(int sockfd, ping_pkt_t *pkt, struct sockaddr_in *dest_addr, struct timeval *timeout)
{
    fd_set readfds;
    struct timeval recv_time = {0, 0};
    struct timeval rtt = {0, 0};
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    int ret = select(sockfd + 1, &readfds, NULL, NULL, timeout);
    if (ret == -1)
    {
        return -1;
    }
    else if (ret)
    {
        uint8_t buffer[1024];
        ssize_t rd = read(sockfd, buffer, sizeof(buffer));
        if (rd > 0)
        {
            gettimeofday(&recv_time, NULL);
            struct iphdr *ip_reply = (struct iphdr *)buffer;
            struct icmphdr *icmp_reply = (struct icmphdr *)(buffer + (ip_reply->ihl << 2));
            unsigned int reply_ttl = ip_reply->ttl;
            // printf("send time: %ld.%06ld\n", pkt->timestamp.tv_sec, pkt->timestamp.tv_usec);
            // printf("recv time: %ld.%06ld\n", recv_time.tv_sec, recv_time.tv_usec);
            timersub(&recv_time, &pkt->timestamp, &rtt);
            print_icmp_reply(icmp_reply, dest_addr, rd - (ip_reply->ihl << 2), reply_ttl, rtt);
        }
        return 1;
    }
    return 0;
}

void print_icmp_reply(struct icmphdr *icmp_reply, struct sockaddr_in *addr, size_t read_bytes, size_t reply_ttl, struct timeval rtt)
{
    const double rtt_seconds = rtt.tv_sec + rtt.tv_usec / 1000000.0;
    if (icmp_reply->type == ICMP_ECHOREPLY || icmp_reply->type == ICMP_ECHO)
        printf("%ld bytes from %s: icmp_seq=%d ttl=%ld time=%.3f ms\n",
               read_bytes, inet_ntoa(addr->sin_addr), ntohs(icmp_reply->un.echo.sequence),
                reply_ttl, rtt_seconds * 1000);
    else if (icmp_reply->type == ICMP_DEST_UNREACH)
        printf("Destination unreachable\n");
    else if (icmp_reply->type == ICMP_TIME_EXCEEDED)
        printf("%ld bytes from %s: Time to live exceeded\n",
               read_bytes, inet_ntoa(addr->sin_addr));
    else
        printf("Unknown type %d\n", icmp_reply->type);
}
