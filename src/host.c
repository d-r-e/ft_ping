#include "../inc/ft_ping.h"

int imroot()
{
    return (getuid() == 0);
}

const char *ft_gethostbyname(const char *name)
{
    struct addrinfo hints = {0};
    int error;

    ft_bzero(&hints, sizeof(hints));
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    error = getaddrinfo(name, NULL, &hints, &g_state.addr_list);
    if (error == 8)
    {
        printf("%s: cannot resolve %s: Unknown host\n", BIN, g_state.hostname);
        exit(68);
    }
    else if (error)
    {
        printf("%s: Error %d: Unknown error getting address info.\n", BIN, error);
        exit(68);
    }

    g_state.host = ft_strdup(inet_ntoa(((struct sockaddr_in *)g_state.addr_list->ai_addr)->sin_addr));
    //printf("%s\n", g_state.host);
    return (g_state.host);
}

