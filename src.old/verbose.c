#include "../inc/ft_ping.h"

int verbose(t_reply *rply)
{
    struct ip *ip;
    struct icmphdr *icmp;

    if (!g_state.v_opt || rply->icmp->icmp_type == ICMP_ECHOREPLY)
        return(0);
    
    ip = (struct ip *)rply->msghdr.msg_iov[0].iov_base;
    icmp = (struct icmphdr *)(rply->icmp + sizeof(struct ip));

    const char *src = inet_ntoa(ip->ip_src);
    const char *dst = g_state.host;
    int src_w = ft_strlen(src);
    int dst_w = ft_strlen(dst);
    // Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data
    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
    printf("%2d %2x  %02x %04x %4x %3x %04x  %02d  %02d %4d ", \
        ip->ip_v, ip->ip_hl, ip->ip_tos, ft_htons(ip->ip_len), htons(ip->ip_id), 0,\
        ip->ip_off, g_state.ttl, ip->ip_p, icmp->checksum);

    printf("%*s %*s \n", src_w, src, dst_w, dst);
    return(0);
}