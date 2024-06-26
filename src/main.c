#include <ft_ping.h>

int main(int argc, char **argv)
{
    char c;
    int ret = 0;
    int verbose = 0;
    unsigned long icmp_count = -1;
    unsigned int ttl = DEFAULT_TTL;
    char *hostname;
    

    static struct option options[] = {
        {"help", no_argument, 0, '?'},
        {"version", no_argument, 0, 'V'},
        {"verbose", no_argument, 0, 'v'},
        {"count", required_argument, 0, 'c'},
        {"ttl", required_argument, 0, 0},
        {0, 0, 0, 0}};

    if (argc < 2)
        usage();

    while ((c = getopt_long(argc, argv, "vc:V?", options, NULL)) != -1)
    {
        switch (c)
        {
        case '?':
            help();
            return strcmp(argv[1], "--help") ? EXIT_FAILURE : EXIT_SUCCESS;
        case 'V':
            version();
            return optarg ? EXIT_SUCCESS : EXIT_FAILURE;
        case 'c':
            is_digit(optarg) ? icmp_count = atol(optarg) : usage();
            break;
        case 'v':
            verbose = 1;
            break;
        case 0:
            is_digit(optarg) ? ttl = atoi(optarg) : usage();
            break;
        default:
            usage();
            return EXIT_FAILURE;
        }
    }
    if (optind >= argc)
        usage();
    while (optind < argc)
    {
        hostname = argv[optind];
        ret += ft_ping(hostname, ttl, icmp_count, verbose);
        optind++;
    }
    return ret;
}