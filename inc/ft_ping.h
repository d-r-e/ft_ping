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
# include <limits.h>
# include "../libft/libft.h"

# define BIN "ft_ping"
# define PING_SZ 64
# define DEFAULT_TIMEOUT 1
# define DEFAULT_TTL 37
typedef struct s_lst {
    double ms;
    struct s_lst * next;
} t_lst;

# define SWAP16(x)			((__uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
# define SWAP32(x)			((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8) | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))
#define ABS(x)				( ( (x) < 0) ? -(x) : (x) )
struct s_state {
// command options
	int			v_opt;
	int			h_opt;
	int			c_opt;
	int			f_opt;
	long int	s_opt;
	int			loop;
// tcp-ip - icmp
	char		*hostname;
	char		*host;
	int			sockfd;
	int			ttl;
	unsigned int	p_transmitted;
	unsigned int	p_received;
	struct addrinfo *addr_list;
	t_lst		*time;
// stats
	double		min;
	double		max;
	double		avg;
	double		msum;
	double		mdev;
	double		sum;
};

struct ping_pkt {
	struct icmphdr icmphdr;
	char msg[PING_SZ - sizeof(struct icmphdr)];
};

typedef struct				s_reply
{
	int						received_bytes;
	struct msghdr			msghdr;
	struct iovec			iov;
	struct icmp				*icmp;
	char					receive_buffer[84];
	char					control[24];
}							t_reply;

typedef struct s_state t_state;

const char	*ft_gethostbyname(const char *name);
float		elapsed(struct timeval a, struct timeval b);
int			ft_ping(void);
void		print_stats(void);
void		ft_exit(int status);

/*
 * MATHUTILS
*/
double	ft_pow(double base, double exp);
double	ft_mean(double data[], int n);
double	ft_variance(double data[], int n);

void ft_sleep( long long sec);
void ft_usleep( long long usec);

extern t_state	g_state;

#endif