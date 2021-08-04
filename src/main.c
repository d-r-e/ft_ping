#include "../inc/ft_ping.h"

static void parse_options(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i)
	{
		if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-v", 2))
			g_state.v_opt = 1;
		if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-h", 2))
			g_state.h_opt = 1;
	}

}

static void usage()
{
	dprintf(STDERR_FILENO, "usage: ft_ping [-hv]\n");
	exit(64);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		usage();
	ft_bzero(&g_state, sizeof(g_state));
	parse_options(argc, argv);
	for (int i = 1; i < argc; ++i)
	{
		printf("%s\n", argv[i]);
	}
	exit(0);
}