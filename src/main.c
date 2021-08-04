#include "../inc/ft_ping.h"

static void usage()
{
	dprintf(STDERR_FILENO, "usage: ft_ping [-hv] host\n");
	exit(64);
}

static void parse_options(int argc, char *argv[])
{
	g_state.host = NULL;
	for (int i = 1; i < argc; ++i)
	{
		if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-v", 2))
			g_state.v_opt = 1;
		else if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-h", 2))
			g_state.h_opt = 1;
		else if (ft_strlen(argv[i]) && argv[i][0] == '-')
			usage();
		else if (ft_strlen(argv[i]) && (argv[i][0]) != '-')
			g_state.host=argv[i];
	}
	if (!g_state.host)
		usage();
}

int main(int argc, char *argv[])
{
	ft_bzero(&g_state, sizeof(g_state));
	parse_options(argc, argv);
	exit(0);
}