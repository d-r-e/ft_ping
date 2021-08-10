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
	answer = ~sum;				/* 1st compliment && truncate to 16 bits */
	return(answer);
}

static void init_reply(t_reply *rply)
{
    rply->msghdr.msg_name = NULL;
	rply->msghdr.msg_iov = &rply->iov;
	rply->iov.iov_base = rply->receive_buffer;
	rply->iov.iov_len = sizeof(rply->receive_buffer);
	rply->msghdr.msg_iovlen = 1;
	rply->msghdr.msg_control = &rply->control;
	rply->msghdr.msg_controllen = sizeof(rply->control);
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
	ft_memcpy(&packet->msg, &(current_time.tv_sec), sizeof(current_time.tv_sec));
	packet->icmphdr.checksum = checksum((uint16_t*)packet, sizeof(*packet));
}

int ft_ping()
{
    struct timeval t0 = {0,0};
    struct timeval t = {0,0};
    ssize_t read;
    struct ping_pkt pckt = {};
    t_reply rply = {};

    ft_bzero(&pckt, sizeof(pckt));
    if (!g_state.loop)
        return(0);
    gettimeofday(&t0, NULL);
    build_ping_packet(&pckt, t0);
    read = sendto(g_state.sockfd, (void*)&pckt, sizeof(pckt), 0, (struct sockaddr *)g_state.addr_list->ai_addr, sizeof(*g_state.addr_list->ai_addr));
    if (read <= 0)
    {
        printf("%s: error: sendto failed\n", BIN);
        return (-1);
    }
    g_state.p_transmitted++;
    ft_bzero(&rply, sizeof (rply));
    // while (!rply.received_bytes)
    init_reply(&rply);
	if (g_state.f_opt)
		printf(".");
    rply.received_bytes = recvmsg(g_state.sockfd, &(rply.msghdr), 0);
    if (rply.received_bytes < 0)
    {
        printf("%s: error: recvfrom failed\n", BIN);
        return (-1);
    }
    if (rply.received_bytes < PING_SZ)
    {
        printf("%s: error: socket closed\n\n", BIN);
        g_state.loop = 0;
        return (-1);
    }
    //strerror(errno);
    gettimeofday(&t, NULL);
    g_state.p_received++;
	if (!g_state.f_opt)
    	printf("%u bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n", \
        	rply.received_bytes - 12, g_state.host, SWAP16(pckt.icmphdr.un.echo.sequence), g_state.ttl, elapsed(t,t0));
	else
		printf("\b");
	return(0);
}