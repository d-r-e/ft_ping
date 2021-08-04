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

typedef struct s_state {
	int			v_opt;
	int			h_opt;
	const char	*host;
}		t_state;

t_state	g_state;

#endif