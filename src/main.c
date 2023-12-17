#include "ft_ping.h"


int main(int argc, char **argv){
	char opt;
	int optind = 0;
	t_state state;

	if (argc < 2)
		print_error("missing host operand");
	ft_bzero(&state, sizeof(t_state));
	ft_bzero(&state.options, sizeof(t_ping_options));
	while (++optind < argc && !ft_strncmp(argv[optind], "-", 1) && ft_strlen(argv[optind > 1]))
	{
		if (!ft_strcmp("--help", argv[optind]) || !ft_strcmp("-h", argv[optind]))
			print_help();
		else if (!ft_strchr("cvV?", argv[optind][1]))
			print_error("invalid option -- 'f'");
		opt = argv[optind][1];
		if (opt == 'c')
		{
			if (argv[optind][2])
				state.options.count = ft_atoi(argv[optind] + 2);
			else if (optind + 1 < argc)
				state.options.count = ft_atoi(argv[++optind]);
			else
				print_error("option requires an argument -- 'c'");
			if (state.options.count < 1)
				print_error("ping count must be greater than 0");
		}
		else if (opt == 'v')
			state.options.verbose = 1;
		else if (opt == 'V')
			print_version();
		else if (opt == '?')
			print_help();
	}
	if (optind >= argc)
		print_error("missing host operand");
	while (optind < argc)
	{
		state.options.host = argv[optind++];
		printf("host: %s\n", state.options.host);
		socket_init(state);
		ft_ping(state);
	}

}