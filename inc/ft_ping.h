#ifndef FT_PING_H
#define FT_PING_H

#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#define DEFAULT_TTL 64
#define PING_PKT_SIZE 64

typedef struct
{
    struct icmphdr hdr;
    char msg[PING_PKT_SIZE - sizeof(struct icmphdr)];
    struct timeval timestamp;
} ping_pkt_t;

typedef struct ping_stats_s
{
    double min_rtt;
    double max_rtt;
    double total_rtt;
    double rtt_squared_sum;
    unsigned int count;
} ping_stats_t;

void usage(void);
void version(void);
void help(void);

int is_digit(const char *str);

int ft_ping(const char *hostname, unsigned int ttl, long count, int verbose);
void print_icmp_reply(struct icmphdr *icmp_reply, struct sockaddr_in *addr, size_t read_bytes, size_t reply_ttl, struct timeval rtt);
int handle_reply(int sockfd, int verbose, ping_pkt_t *pkt, struct sockaddr_in *dest_addr, struct timeval *timeout, ping_stats_t *stats);
void print_stats(const char *hostname, unsigned int total_sent, unsigned int total_received, ping_stats_t *stats);

#endif // FT_PING_H