#include "../inc/ft_ping.h"

uint16_t
checksum(uint16_t *addr, int len)
{
	int nleft, sum;
	uint16_t *w;
	union {
		uint16_t	us;
		u_char	uc[2];
	} last;
	uint16_t answer;

	nleft = len;
	sum = 0;
	w = addr;

	/*
	 * The algorithm is simple, using a 32 bit accumulator (sum), we add
	 * sequential 16 bit words to it, and at the end, fold back all the
	 * carry bits from the top 16 bits into the lower 16 bits.
	 */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* append an odd byte for padding, if necessary */
	if (nleft == 1) {
		last.uc[0] = *(u_char *)w;
		last.uc[1] = 0;
		sum += last.us;
	}

	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* magic trick: truncate to 16 bits */
	return(answer);
}

static void build_ping_packet(struct ping_pkt *packet, struct timeval current_time)
{
    static int msg_count = 0;

	ft_bzero(packet, sizeof(packet));
	packet->icmphdr.type = ICMP_ECHO;
	packet->icmphdr.code = 0;
	packet->icmphdr.un.echo.sequence = SWAP16(msg_count);
	packet->icmphdr.un.echo.id = (getpid()>>8) | (getpid()<<8);
    msg_count++;
    (void)current_time;
	//ft_memcpy(&packet->icmphdr.un.echo.sequence, &(current_time.tv_sec), sizeof(current_time.tv_sec));
	packet->icmphdr.checksum = checksum((uint16_t*)packet, sizeof(*packet));

    // ft_bzero(&pckt, sizeof(pckt));
    // pckt.hdr.type = ICMP_ECHO; // 8
    // pckt.hdr.un.echo.id = getpid();
        
    // for (i = 0; i < sizeof(pckt.msg)-1; i++ )
    //     pckt.msg[i] = i + '0';
    // //printf("%.56s\n", pckt.msg);
    // pckt.msg[i] = 0;
    // pckt.hdr.un.echo.sequence = msg_count++;
    // pckt.hdr.checksum = checksum( (uint16_t*)&pckt, sizeof(pckt) );
    // int nada =checksum((unsigned char*)&pckt.hdr, sizeof(pckt.hdr));
    //printf("checksum %d\n", packet->icmphdr.checksum);
}



int ft_ping()
{
    struct timeval t0 = {0,0};
    struct timeval t = {0,0};
    ssize_t read;
    struct ping_pkt pckt = {};
    t_reply rply;

    ft_bzero(&pckt, sizeof(pckt));
    if (!g_state.loop)
        return(0);
    gettimeofday(&t0, NULL);
    build_ping_packet(&pckt, t0);
    read = sendto(g_state.sockfd, (void*)&pckt, sizeof(pckt), 0, (struct sockaddr *)g_state.addr_list->ai_addr, sizeof(*g_state.addr_list->ai_addr));
    if (read < 1)
    {
        printf("%s: reador: sendto failed\n", BIN);
        return (-1);
    }
    g_state.p_transmitted++;
    usleep(1000);
    read = recvmsg(g_state.sockfd, &(rply.msghdr), 0);
    //read = recvfrom(g_state.sockfd, (void*)&pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &addr_len);
    if (read < 1)
    {
        printf("%s: reador: recvfrom failed\n", BIN);
        return (-1);
    }
    gettimeofday(&t, NULL);
    g_state.p_received++;
    printf("%lu bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n", \
        read, g_state.host, SWAP16(pckt.icmphdr.un.echo.sequence), g_state.ttl, elapsed(t,t0));

	return(0);
}