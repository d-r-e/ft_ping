#include "../inc/ft_ping.h"

const char *icmp_type_str(int type)
{
    switch (type)
    {
    case ICMP_ECHOREPLY:
        return ("ICMP_ECHOREPLY");
    case ICMP_DEST_UNREACH:
        return ("ICMP_DEST_UNREACH");
    case ICMP_SOURCE_QUENCH:
        return ("ICMP_SOURCE_QUENCH");
    case ICMP_REDIRECT:
        return ("ICMP_REDIRECT");
    case ICMP_ECHO:
        return ("ICMP_ECHO");
    case ICMP_TIME_EXCEEDED:
        return ("ICMP_TIME_EXCEEDED");
    case ICMP_PARAMETERPROB:
        return ("ICMP_PARAMETERPROB");
    case ICMP_TIMESTAMP:
        return ("ICMP_TIMESTAMP");
    case ICMP_TIMESTAMPREPLY:
        return ("ICMP_TIMESTAMPREPLY");
    case ICMP_INFO_REQUEST:
        return ("ICMP_INFO_REQUEST");
    case ICMP_INFO_REPLY:
        return ("ICMP_INFO_REPLY");
    case ICMP_ADDRESS:
        return ("ICMP_ADDRESS");
    case ICMP_ADDRESSREPLY:
        return ("ICMP_ADDRESSREPLY");
    default:
        return ("UNKNOWN");
    }
}

const char *icmp_code_str(int code)
{
    switch (code)
    {
    case ICMP_NET_UNREACH:
        return ("ICMP_NET_UNREACH");
    case ICMP_HOST_UNREACH:
        return ("ICMP_HOST_UNREACH");
    case ICMP_PROT_UNREACH:
        return ("ICMP_PROT_UNREACH");
    case ICMP_PORT_UNREACH:
        return ("ICMP_PORT_UNREACH");
    case ICMP_FRAG_NEEDED:
        return ("ICMP_FRAG_NEEDED");
    case ICMP_SR_FAILED:
        return ("ICMP_SR_FAILED");
    case ICMP_NET_UNKNOWN:
        return ("ICMP_NET_UNKNOWN");
    case ICMP_HOST_UNKNOWN:
        return ("ICMP_HOST_UNKNOWN");
    case ICMP_HOST_ISOLATED:
        return ("ICMP_HOST_ISOLATED");
    case ICMP_NET_ANO:
        return ("ICMP_NET_ANO");
    case ICMP_HOST_ANO:
        return ("ICMP_HOST_ANO");
    case ICMP_NET_UNR_TOS:
        return ("ICMP_NET_UNR_TOS");
    case ICMP_HOST_UNR_TOS:
        return ("ICMP_HOST_UNR_TOS");
    case ICMP_PKT_FILTERED:
        return ("ICMP_PKT_FILTERED");
    case ICMP_PREC_VIOLATION:
        return ("ICMP_PREC_VIOLATION");
    case ICMP_PREC_CUTOFF:
        return ("ICMP_PREC_CUTOFF");
    default:
        return ("UNKNOWN");
    }
}

void print_ip(struct ip *ip)
{
    // printf("IP version: %d\n", ip->ip_v);
    // printf("IP header length: %d\n", ip->ip_hl);
    // printf("IP type of service: %d\n", ip->ip_tos);
    // printf("IP total length: %d\n", ip->ip_len);
    // printf("IP identification: %d\n", ip->ip_id);
    // printf("IP fragment offset: %d\n", ip->ip_off);
    // printf("IP time to live: %d\n", ip->ip_ttl);
    // printf("IP protocol: %d\n", ip->ip_p);
    // printf("IP checksum: %d\n", ip->ip_sum);
    printf("IP source: %s\n", inet_ntoa(ip->ip_src));
    printf("IP destination: %s\n", inet_ntoa(ip->ip_dst));
}

void print_icmp(struct icmphdr *icmp)
{

    printf("ICMP type: %s (%02x -> %02d)\n", icmp_type_str(icmp->type), icmp->type, icmp->type);
    if (icmp->type != ICMP_ECHOREPLY)
        printf("ICMP code: %d\n", icmp->code);
    // printf("ICMP checksum: %d\n", icmp->checksum);
    // printf("ICMP id: %d\n", SWAP16(icmp->un.echo.id));
    // printf("ICMP sequence: %d\n", icmp->un.echo.sequence);
}

void print_icmp_hex(struct icmphdr *icmp)
{
    for (size_t i = 0; i < sizeof(icmp); ++i)
        printf("%02x ", ((unsigned char *)icmp)[i]);
    printf("\n");
}

void print_iovec(t_reply *t)
{
    for (size_t i = 0; i < t->msghdr.msg_iovlen; ++i)
    {
        // 	printf("iovec[%zu].iov_base = %p\n", i, t->msghdr.msg_iov[i].iov_base);
        // 	printf("iovec[%zu].iov_len = %zu\n", i, t->msghdr.msg_iov[i].iov_len);
        // print_ip((struct ip *)t->msghdr.msg_iov[i].iov_base);
        print_icmp((struct icmphdr *)(t->msghdr.msg_iov[i].iov_base + sizeof(struct ip)));
        print_icmp_hex((struct icmphdr *)(t->msghdr.msg_iov[i].iov_base + sizeof(struct ip)));
    }
}

static void init_reply(t_reply *rply)
{
    ft_bzero(rply, sizeof(rply));
    rply->msghdr.msg_iov = &rply->iov;
    rply->msghdr.msg_iovlen = 1;
    rply->iov.iov_base = rply->receive_buffer;
    rply->iov.iov_len = sizeof(rply->receive_buffer);
    rply->msghdr.msg_control = &rply->control;
    rply->msghdr.msg_controllen = sizeof(rply->control);
    rply->icmp = rply->iov.iov_base + sizeof(struct ip);
    // print_rply_hex(rply);
}

int receive_reply()
{
    t_reply rply = {};
    struct icmphdr *icmp;

    init_reply(&rply);
    rply.received_bytes = recvmsg(g_state.sockfd, &rply.msghdr, 0);
    if (errno)
    {
        dprintf(2, "recvmsg: %s\n", "error");
        return (1);
    }
    icmp = (struct icmphdr *)(rply.icmp);
    if (icmp->type == ICMP_ECHOREPLY)
    {
        gettimeofday(&g_state.t, NULL);
        if (g_state.f_opt)
        {
            printf("\n");
        }
        g_state.p_received++;
        g_state.c_opt--;
    }
    else if (icmp->type == ICMP_TIME_EXCEEDED)
    {
        struct ip *ip = (struct ip *)(rply.msghdr.msg_iov[0].iov_base);
        getnameinfo((struct sockaddr *)&ip->ip_src, sizeof(ip->ip_src), g_state.hostname, sizeof(g_state.hostname), NULL, 0, NI_NAMEREQD);
        printf("%ld bytes from %s: Time to live excceeded\n", \
                rply.received_bytes, \
                inet_ntoa(ip->ip_src));
    }
    // print_iovec(&rply);
    short seq = ft_htons((short)((struct icmphdr *)(rply.msghdr.msg_iov[0].iov_base + sizeof(struct ip)))->un.echo.sequence);
    gettimeofday(&g_state.t, NULL);
    if (!g_state.f_opt && icmp->type == ICMP_ECHOREPLY)
    {
        printf("%lu bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
               rply.received_bytes - IP_HDR_LEN, g_state.host, seq, g_state.ttl, elapsed(g_state.t, g_state.t0));
    }
    else
        printf("\b");
    if (!g_state.c_opt)
    {
        print_stats();
        ft_exit(0);
    }
    return (0);
}