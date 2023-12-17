#include "../inc/ft_ping.h"
#include <arpa/inet.h>

const char *ft_gethostbyname(const char *name)
{
    struct addrinfo hints, *res;
    int error;
    char addrstr[INET6_ADDRSTRLEN];

    ft_bzero(&hints, sizeof(hints));
    hints.ai_flags = 0;
    hints.ai_family = AF_UNSPEC; // Use AF_UNSPEC for both IPv4 and IPv6
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = 0; // Set to 0 to handle both ICMP and ICMPv6

    error = getaddrinfo(name, NULL, &hints, &g_state.addr_list);
    if (error)
    {
        printf("%s: Error in getaddrinfo: %s\n", BIN, gai_strerror(error));
        exit(68);
    }

    for (res = g_state.addr_list; res != NULL; res = res->ai_next)
    {
        void *addr;

        // Check the IP version and adapt accordingly
        if (res->ai_family == AF_INET) // IPv4
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
            addr = &(ipv4->sin_addr);
            g_state.protocol = IPPROTO_ICMP;
        }
        else // IPv6
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res->ai_addr;
            addr = &(ipv6->sin6_addr);
            g_state.protocol = IPPROTO_ICMPV6;
        }

        // Convert the IP to a string and copy it
        inet_ntop(res->ai_family, addr, addrstr, sizeof(addrstr));
        g_state.host = ft_strdup(addrstr);
        break; // Break after the first address is processed
    }

    if (g_state.host == NULL)
    {
        printf("%s: No addresses found for host %s\n", BIN, name);
        exit(68);
    }

    return (g_state.host);
}