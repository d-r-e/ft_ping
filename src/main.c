#include <ft_ping.h>

static struct option options[] = {
    {"help", no_argument, 0, '?'},
    {"version", no_argument, 0, 'V'},
    {"verbose", no_argument, 0, 'v'},
    {"count", required_argument, 0, 'c'},
    {"timeout", required_argument, 0, 'w'},
    {"ttl", required_argument, 0, 0},
    {0, 0, 0, 0}};

int main(int argc, char **argv)
{
    char c;
    unsigned long icmp_count = -1;
    int timeout = -1;
    unsigned int ttl = DEFAULT_TTL;
    char *hostname;

    if (argc < 2)
        usage();

    while ((c = getopt_long(argc, argv, "vc:w:V?", options, NULL)) != -1)
    {
        switch (c)
        {
        case '?':
            help();
            return strcmp(argv[1], "--help") ? EXIT_FAILURE : EXIT_SUCCESS;
        case 'V':
            version();
            return EXIT_SUCCESS;
        case 'c':
            is_digit(optarg) ? icmp_count = atol(optarg) : usage();
            break;
        case 'w':
            is_digit(optarg) ? timeout = atoi(optarg) : usage();
            break;
        case 0:
            if (strlen(options[optind].name) >= 3 &&
                !strncmp(options[optind].name, "ttl", 3)
                && is_digit(optarg))
                ttl = atoi(optarg);
            break;
        default:
            usage();
            return EXIT_FAILURE;
        }
    }
    if (optind >= argc)
        usage();
    hostname = argv[optind];

    return ft_ping(hostname, ttl, icmp_count, timeout);
}