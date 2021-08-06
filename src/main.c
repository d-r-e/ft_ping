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
		if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-v", 3))
			g_state.v_opt = 1;
		else if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-o", 3))
			g_state.o_opt = 1;
		else if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-f", 3))
		{
			if (imroot() == 0)
			{
				printf("%s: -f flag: Operation not permitted\n", BIN);
				exit (77);
			}
			g_state.f_opt = 1;
		} else if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-h", 3))
			g_state.h_opt = 1;
		else if (ft_strlen(argv[i]) && argv[i][0] == '-')
			usage();
		else if (ft_strlen(argv[i]) && (argv[i][0]) != '-')
			g_state.hostname=argv[i];
	}
	if (!g_state.hostname)
		usage();
}

static void init_state()
{
	ft_bzero(&g_state, sizeof(g_state));
	g_state.s_opt = PING_SZ;
	g_state.loop = 1;
	g_state.ttl = 37;
}

static void sighandler()
{
	g_state.loop = 0;
	printf("\r");
}

static int main_loop(){
	printf("%s: %s (%s): %ld(%ld) data bytes\n", "PING", \
		g_state.hostname, \
		(ft_strlen(g_state.host) != 0) ? g_state.host : g_state.hostname, \
		g_state.s_opt, \
		g_state.s_opt + sizeof(struct icmphdr) \
	);
	if (getuid())
		g_state.sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	else
		g_state.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (g_state.sockfd < 0)
    {
        printf("%s: error: no socket\n", BIN);
        return (-1);
    }
	do
	{
		ft_ping();
	} while (!g_state.o_opt && g_state.loop == 1);
    close(g_state.sockfd);
	printf("--- %s ft_ping statistics ---\n", g_state.hostname);
    return (0);
}

static void cleanup(int n)
{
	free(g_state.host);
	//freeaddrinfo(g_state.addr_list);
	exit(n);
}

t_state g_state;

int main(int argc, char *argv[])
{
	init_state();
	parse_options(argc, argv);
	signal(SIGINT, sighandler);
	ft_gethostbyname(g_state.hostname);
	main_loop();
	
	cleanup(0);
}