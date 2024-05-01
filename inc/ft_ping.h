#ifndef FT_PING_H
#define FT_PING_H

#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define DEFAULT_TTL 64
#define PING_PKT_SIZE 64

typedef struct
{
    struct icmphdr hdr;
    char msg[PING_PKT_SIZE - sizeof(struct icmphdr)];
} ping_pkt_t;

void usage(void);
void version(void);
void help(void);

int is_digit(const char *str);

int ft_ping(const char *hostname, unsigned int ttl, long count, unsigned int timeout_ms, int verbose);
void print_icmp_reply(struct icmphdr *icmp_reply, struct sockaddr_in *addr, size_t read_bytes, size_t reply_ttl);
int handle_reply(int sockfd, struct sockaddr_in *dest_addr, struct timeval *timeout);
void print_stats();

// Global variables
extern unsigned int total_sent;
extern unsigned int total_received;

#endif // FT_PING_H