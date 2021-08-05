#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <signal.h>
# include <sys/types.h>
# include <netdb.h>
# include "../libft/libft.h"

# define BIN "ft_ping"

typedef struct s_state {
	int			v_opt;
	int			h_opt;
	int			o_opt;
	int			f_opt;
	char	*hostname;
	char	*host;
	struct addrinfo *addr_list;
}		t_state;


const char *ft_gethostbyname(const char *name);

t_state	g_state;

#endif