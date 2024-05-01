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
