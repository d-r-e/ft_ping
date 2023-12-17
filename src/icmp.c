#include "ft_ping.h"

static unsigned short checksum(void *address, size_t len)
{
    unsigned short *src;
    unsigned long sum;

    src = (unsigned short *)address;
    sum = 0;
    while (len > 1) 
    {
        sum += *src;
        src++;
        len -= sizeof(short);
    }
    if (len)
        sum += *(unsigned char *)src;
    sum = (sum >> 16) + (sum & 0xffff);
    return (sum);
}

void build_ping_packet(t_icmp_packet *packet, struct timeval current_time)
{
    packet = malloc(sizeof(t_icmp_packet));
    ft_bzero(packet, sizeof(t_icmp_packet));
    packet->icmphdr.type = ICMP_ECHO;
    packet->icmphdr.un.echo.id = getpid();
    packet->icmphdr.un.echo.sequence = 1;
    packet->icmphdr.checksum = checksum(packet, sizeof(t_icmp_packet));
    packet->msg[0] = current_time.tv_sec;
    packet->msg[1] = current_time.tv_usec;
    packet->msg[2] = 0;
}