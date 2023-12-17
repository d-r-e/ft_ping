#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <limits.h>
#include <errno.h>

#include "../libft/libft.h"

#define BIN "ft_ping"
#define DEFAULT_TTL 64
typedef struct s_ping_options
{
	int verbose;
	int help;
	int usage;
	int version;
	int numeric;
	int interval;
	int count;
	int flood;
	int timeout;
	int ttl_set;
	char *host;
} t_ping_options;

typedef struct s_state
{
	struct addrinfo *addr_list;
	int protocol;
	int sockfd;
	int datalen;
	int ttl;
	int ttl_set;
	int ttl_max;
	int ttl_min;
	int ttl_sum;
	int ttl_count;
	int ttl_avg;
	struct timeval t0;
	struct timeval t1;
	int loop;
	int tx;
	int rx;

	t_ping_options options;
} t_state;

#define SWAP16(x) ((x >> 8) | (x << 8))

typedef struct s_icmp_packet
{
	struct icmphdr icmphdr;
	uint8_t type; /* message type */
	uint8_t code; /* type sub-code */
	uint16_t checksum;
	union
	{
		struct
		{
			uint16_t id;
			uint16_t sequence;
		} echo;			  /* echo datagram */
		uint32_t gateway; /* gateway address */
		struct
		{
			uint16_t __glibc_reserved;
			uint16_t mtu;
		} frag; /* path mtu discovery */
	} un;
	char msg[56];
} t_icmp_packet;

void print_version(void);
void print_error(char *message);
void print_help(void);
int socket_init(t_state state);
void ft_ping(t_state state);
void build_ping_packet(t_icmp_packet *packet, struct timeval current_time);

#endif