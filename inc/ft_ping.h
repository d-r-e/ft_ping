#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"

typedef struct s_state {
	int	v_opt;
	int	h_opt;
}		t_state;

t_state	g_state;

#endif