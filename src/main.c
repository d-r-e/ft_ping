#include "../inc/ft_ping.h"

static void usage()
{
	dprintf(2, "usage: ft_ping [-hv] host\n");
	exit(64);
}

static void parse_options(int argc, char *argv[])
{
	g_state.hostname = NULL;
	for (int i = 1; i < argc; ++i)
	{
		if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-v", 3))
			g_state.v_opt = 1;
		else if (!ft_strncmp(argv[i], "-c", ft_strlen("-c") + 1))
		{
			if (i + 1 < argc)
				g_state.c_opt = ft_atoi(argv[++i]);
			else
				usage();
			if (g_state.c_opt <= 0)
				usage();
		}
		else if (!ft_strncmp(argv[i], "-i", ft_strlen("-i") + 1))
		{
			if (i + 1 < argc)
				g_state.i_opt = ft_atoi(argv[++i]);
			else
				usage();
			if (g_state.i_opt < 0)
				usage();
		}
		else if (!ft_strncmp(argv[i], "--ttl", ft_strlen("--ttl") + 1))
		{
			if (i + 1 < argc)
				g_state.ttl = ft_atoi(argv[++i]);
			else
				g_state.ttl = DEFAULT_TTL;
			if (g_state.ttl <= 0)
				usage();
		}
		else if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-f", 3))
		{
			if (getuid())
			{
				printf("%s: -f flag: Operation not permitted\n", BIN);
				exit(77);
			}
			g_state.f_opt = 1;
		}
		else if (ft_strlen(argv[i]) > 1 && !ft_strncmp(argv[i], "-h", 3))
			g_state.h_opt = 1;
		else if (ft_strlen(argv[i]) && argv[i][0] == '-')
			usage();
		else if (ft_strlen(argv[i]) && (argv[i][0]) != '-')
			g_state.hostname = argv[i];
	}
	if (!g_state.hostname)
		usage();
}

static void init_state()
{
	ft_bzero(&g_state, sizeof(g_state));
	g_state.time = NULL;
	g_state.c_opt = -1;
	g_state.s_opt = PING_SZ;
	g_state.ttl = 37;
	g_state.loop = 1;
	g_state.i_opt = 1;
}

static void sighandler(int c)
{
	if (c == SIGINT)
	{
		g_state.loop = 0;
		printf("\r");
	}
	else if (c == SIGALRM)
	{
		g_state.sleep = 1;
		printf("ding)");
	}
}

static int get_socket()
{
	struct timeval timeout = {1, 0};

	g_state.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_state.sockfd < 0)
	{
		printf("%s: error: no socket\n", BIN);
		return (-1);
	}
	if ((setsockopt(g_state.sockfd, IPPROTO_IP, IP_TTL, &(g_state.ttl), sizeof(g_state.ttl))) == -1)
		return (-1);
	if ((setsockopt(g_state.sockfd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeout, sizeof(timeout))) == -1)
		return (-1);
	return (g_state.sockfd);
}

static int main_loop()
{
	printf("%s: %s (%s): %ld(%ld) data bytes\n", "PING",
		   g_state.hostname,
		   (ft_strlen(g_state.host) != 0) ? g_state.host : g_state.hostname,
		   g_state.s_opt,
		   g_state.s_opt + sizeof(struct icmphdr));
	if (g_state.c_opt == 0 || get_socket() < 0)
		return (-1);
	do
	{
		if (g_state.c_opt == INT32_MIN)
			g_state.c_opt = -1;
		ft_ping();
		--g_state.c_opt;

		if (g_state.c_opt && !g_state.f_opt)
			ft_sleep(g_state.i_opt);
	} while (g_state.c_opt && g_state.loop == 1);
	close(g_state.sockfd);
	print_stats();
	return (0);
}

void ft_exit(int status)
{
	free(g_state.host);
	freeaddrinfo(g_state.addr_list); // is this function forbidden?
	exit(status);
}

t_state g_state;

int main(int argc, char *argv[])
{
	init_state();
	parse_options(argc, argv);
	signal(SIGINT, sighandler);
	ft_gethostbyname(g_state.hostname);
	main_loop();
	ft_exit(0);
}