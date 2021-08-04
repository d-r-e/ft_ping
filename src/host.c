#include "../inc/ft_ping.h"

const char *ft_gethostbyname(const char *name)
{
    struct addrinfo hints = {0};
    int error;

    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    error = getaddrinfo(name, NULL , &hints, &g_state.addr_list);
    if (error)
        return (NULL);
    g_state.host = inet_ntoa(((struct sockaddr_in *)g_state.addr_list->ai_addr)->sin_addr);

    return (g_state.host);
}