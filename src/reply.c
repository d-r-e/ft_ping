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