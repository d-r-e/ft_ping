#include <ft_ping.h>

/// @brief This function generates advances statistics of the IP packet when the ICMP fails using the verbose option
/// @param ip
///
/// VERSION: IP V4 or V6
/// IHL: Internet Header Length
/// TOS: Type of Service (Determines priority of the packet and how it should be handled)
/// LENGTH: Total Length of the IP Packet
/// ID: Identification
/// FLAG: Flags (i.e. Don't Fragment, More Fragments)
/// OFFSET: Fragment Offset (Offset of the fragment relative to the beginning of the original unfragmented IP datagram)
/// TTL: Time to Live (Number of hops before the packet is discarded)
/// PROTOCOL: Protocol (i.e. ICMP, TCP, UDP)
/// CHECKSUM: Header Checksum
/// SOURCEIP: Source IP Address
/// DESTINATIONIP: Destination IP Address
static void dump_ip_header(struct iphdr *ip)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->daddr), dst_ip, INET_ADDRSTRLEN);

    printf("IP Header Dump:\n");
    printf("  Version| IHL | TOS |Length|    ID |Flg| Offset | TTL | Protocol| Checksum |   Source IP   | Destination IP|\n");
    printf("  %7x|%5x|%5x|%6x|%7x|%3x|%8x|%5x|%9x|%10x|%15s|%15s|\n",
           ip->version, ip->ihl, ip->tos, ntohs(ip->tot_len),
           ntohs(ip->id), ntohs(ip->frag_off) >> 13, ntohs(ip->frag_off) & 0x1FFF,
           ip->ttl, ip->protocol, ntohs(ip->check),
           src_ip, dst_ip);
}

/// @brief Receives ICMP reply and updates statistics of the ping session
/// @param sockfd
/// @param pkt
/// @param dest_addr
/// @param timeout
/// @param stats
/// @return 1 if success, 0 otherwise
int handle_reply(int sockfd, int verbose, ping_pkt_t *pkt, struct sockaddr_in *dest_addr, struct timeval *timeout, ping_stats_t *stats)
{
    fd_set readfds;
    struct timeval recv_time = {0, 0};
    struct timeval rtt = {0, 0};
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    int ret = select(sockfd + 1, &readfds, NULL, NULL, timeout);
    if (ret == -1)
        return ret;
    else if (ret)
    {
        uint8_t buffer[sizeof(struct iphdr) + sizeof(ping_pkt_t)];
        ssize_t rd = read(sockfd, buffer, sizeof(buffer));
        unsigned int reply_ttl;
        if (rd > 0)
        {
            gettimeofday(&recv_time, NULL);
            struct iphdr *ip_reply = (struct iphdr *)buffer;
            struct icmphdr *icmp_reply = (struct icmphdr *)(buffer + (ip_reply->ihl << 2));
            if (icmp_reply->type == ICMP_ECHOREPLY || icmp_reply->type == ICMP_ECHO)
            { // Only process echo replies
                reply_ttl = ip_reply->ttl;
                timersub(&recv_time, &pkt->timestamp, &rtt);
                double rtt_ms = rtt.tv_sec * 1000.0 + rtt.tv_usec / 1000.0;

                if (rtt_ms < stats->min_rtt)
                    stats->min_rtt = rtt_ms;
                if (rtt_ms > stats->max_rtt)
                    stats->max_rtt = rtt_ms;
                stats->total_rtt += rtt_ms;
                stats->rtt_squared_sum += rtt_ms * rtt_ms;
                stats->count++;

                ret = 1;
            }
            reply_ttl = 0;
            print_icmp_reply(icmp_reply, dest_addr, rd - (ip_reply->ihl << 2), reply_ttl, rtt);
            if (verbose)
            {
                ret = 0;
                dump_ip_header(ip_reply);
            }
        }
        return ret;
    }
    return 0;
}

/// @brief Print ICMP reply; either ECHO_REPLY, DEST_UNREACH, or TIME_EXCEEDED
/// @param icmp_reply
/// @param addr
/// @param read_bytes
/// @param reply_ttl
/// @param rtt
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

/// @brief Prints summary of ping session
/// @param hostname
/// @param total_sent
/// @param total_received
/// @param stats
void print_stats(const char *hostname, unsigned int total_sent, unsigned int total_received, ping_stats_t *stats)
{
    printf("--- %s ping statistics ---\n", hostname);
    double packet_loss = total_sent > 0 ? ((total_sent - total_received) / (double)total_sent) * 100.0 : 0.0;
    printf("%u packets transmitted, %u received, %.1f%% packet loss\n",
           total_sent, total_received, packet_loss);

    double avg_rtt = stats->count > 0 ? stats->total_rtt / stats->count : 0.0;
    double variance = stats->count > 0 ? (stats->rtt_squared_sum / stats->count) - (avg_rtt * avg_rtt) : 0.0;
    double stddev = sqrt(variance);
    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
           isfinite(stats->min_rtt) ? stats->min_rtt : 0.0,
           isfinite(avg_rtt) ? avg_rtt : 0.0,
           isfinite(stats->max_rtt) ? stats->max_rtt : 0.0,
           isfinite(stddev) ? stddev : 0.0);
}