#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <signal.h>
# include <netinet/ip_icmp.h>
# include <sys/types.h>
# include <netdb.h>
# include "../libft/libft.h"

# define BIN "ft_ping"
# define PING_SZ 56

struct s_state {
// command options
	int			v_opt;
	int			h_opt;
	int			o_opt;
	int			f_opt;
	long int	s_opt;
	int			loop;
// tcp-ip - icmp
	char		*hostname;
	char		*host;
	int			sockfd;
	int			ttl;
	struct addrinfo *addr_list;
};

struct ping_pkt {
	struct icmphdr *hdr;
	char msg[PING_SZ - sizeof(struct icmphdr)];
};

typedef struct s_state t_state;

const char	*ft_gethostbyname(const char *name);
float		timestamp(struct timeval a, struct timeval b);
int			ft_ping(void);

extern t_state	g_state;

#endif