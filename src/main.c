#include "../inc/ft_ping.h"

static void usage()
{
	dprintf(STDERR_FILENO, "usage: ft_ping [-hv] host\n");
	exit(64);
}

static void parse_options(int argc, char *argv[])
{
	g_state.hostname = NULL;
	for (int i = 1; i < argc; ++i)
	{
		if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-v", 2))
			g_state.v_opt = 1;
		if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-o", 2))
			g_state.o_opt = 1;
		else if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-h", 2))
			g_state.h_opt = 1;
		else if (ft_strlen(argv[i]) && argv[i][0] == '-')
			usage();
		else if (ft_strlen(argv[i]) && (argv[i][0]) != '-')
			g_state.hostname=argv[i];
	}
	if (!g_state.hostname)
		usage();
}

static void statistics()
{
	printf("\r--- %s ft_ping statistics ---\n", g_state.hostname);
	exit(0);
}

// static void leaks()
// {
// 	system("leaks ft_ping");
// }

static void ft_ping(){
	printf("FT_PING: %s (%s): %d data bytes\n", g_state.hostname, g_state.host, 56);
}

int main(int argc, char *argv[])
{
	//atexit(leaks);
	ft_bzero(&g_state, sizeof(g_state));
	parse_options(argc, argv);
	signal(SIGINT, statistics);
	ft_gethostbyname(g_state.hostname);
	ft_ping();
	while(42)
		;
	exit(0);
}