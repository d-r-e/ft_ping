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
			if (getuid())
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
	g_state.time = NULL;
	g_state.s_opt = PING_SZ;
	g_state.ttl = 37;
	g_state.loop = 1;
}

static void sighandler()
{
	g_state.loop = 0;
	printf("\r");
}

static int get_socket()
{
	struct timeval timeout;

	timeout.tv_sec = DEFAULT_TIMEOUT;
    timeout.tv_usec = 0;
	g_state.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (g_state.sockfd < 0)
    {
        printf("%s: error: no socket\n", BIN);
        return (-1);
    }
	if (setsockopt(g_state.sockfd, SOL_IP, IP_TTL, 
               &timeout, sizeof(timeout)) != 0)
    {
        printf("%s: \nSetting socket options \
                 to TTL failed!\n", BIN);
        return (-1);
    } 
    // setting timeout of recv setting
    setsockopt(g_state.sockfd, SOL_SOCKET, SO_RCVTIMEO,
                   (const char*)&timeout, sizeof timeout);
	return (g_state.sockfd);
}

static int main_loop(){
	printf("%s: %s (%s): %ld(%ld) data bytes\n", "PING", \
		g_state.hostname, \
		(ft_strlen(g_state.host) != 0) ? g_state.host : g_state.hostname, \
		g_state.s_opt, \
		g_state.s_opt + sizeof(struct icmphdr) \
	);
	if (get_socket() < 0)
		return (-1);
	do
	{
		ft_ping();
		if (!g_state.o_opt)
		    ft_sleep(1);

	} while (!g_state.o_opt && g_state.loop == 1);
    close(g_state.sockfd);
	print_stats();
    return (0);
}

void ft_exit(int status)
{
	free(g_state.host);
	freeaddrinfo(g_state.addr_list);
	exit(status);
}

t_state g_state;

int main(int argc, char *argv[])
{
	// double data[] ={9,9,9,9,9,10};

	// printf("%f\n%f\n", ft_pow(2,3), ft_pow(2,1));
	// printf("%f\n%f\n", ft_pow(1,9), ft_pow(0,6));
	// printf("mean: %f\n", ft_mean(data, 6));
	init_state();
	parse_options(argc, argv);
	signal(SIGINT, sighandler);
	ft_gethostbyname(g_state.hostname);
	main_loop();
	
	ft_exit(0);
}