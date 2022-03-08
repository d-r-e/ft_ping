#include "../inc/ft_ping.h"

// uint16_t
// checksum(uint16_t *addr, size_t len)
// {
// 	int nleft, sum;
// 	uint16_t *w;
// 	union
// 	{
// 		uint16_t us;
// 		unsigned char uc[2];
// 	} last;
// 	uint16_t answer;

// 	nleft = len;
// 	sum = 0;
// 	w = addr;

// 	/*
// 	 * The algorithm is simple, using a 32 bit accumulator (sum), we add
// 	 * sequential 16 bit words to it, and at the end, fold back all the
// 	 * carry bits from the top 16 bits into the lower 16 bits.
// 	 */
// 	while (nleft > 1)
// 	{
// 		sum += *w++;
// 		nleft -= 2;
// 	}

// 	/* append an odd byte for padding, if necessary */
// 	if (nleft == 1)
// 	{
// 		last.uc[0] = *(unsigned char *)w;
// 		last.uc[1] = 0;
// 		sum += last.us;
// 	}

// 	/* add back carry outs from top 16 bits to low 16 bits */
// 	sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
// 	sum += (sum >> 16);					/* add carry */
// 	answer = ~sum;						/* 1st compliment && truncate to 16 bits */
// 	return (answer);
// }

static unsigned short checksum(void *address, size_t len)
{
	unsigned short	*src;
	unsigned long	sum;

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
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((unsigned short)~sum);
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
	rply->icmp = rply->iov.iov_base + sizeof(struct icmphdr);
	//print_rply_hex(rply);
}

// static void print_hex_packet(struct ping_pkt *pkt)
// {
// 	size_t i;

// 	i = 0;
// 	while (i < sizeof(pkt))
// 	{
// 		printf("%02x ", ((unsigned char *)&pkt)[i]);
// 		i++;
// 	}
// 	printf("\n");
// }


static void build_ping_packet(struct ping_pkt *packet, struct timeval current_time)
{
	ft_bzero(packet, sizeof(packet));
	packet->icmphdr.type = ICMP_ECHO;
	packet->icmphdr.code = 0;
	packet->icmphdr.un.echo.id = SWAP16(getpid());
	packet->icmphdr.un.echo.sequence = SWAP16(g_state.p_transmitted);
	(void)current_time;
	ft_memcpy(&packet->msg, &current_time.tv_sec, sizeof(current_time.tv_sec));
	for (unsigned char i = 0; i < 40; ++i)
		packet->msg[i + 16] = i;
	packet->icmphdr.checksum = (checksum(&packet->icmphdr, PING_SZ));
	// print_packet_fields(*packet);
}



 void print_ip(struct ip *ip)
{
	printf("IP version: %d\n", ip->ip_v);
	printf("IP header length: %d\n", ip->ip_hl);
	printf("IP type of service: %d\n", ip->ip_tos);
	printf("IP total length: %d\n", ip->ip_len);
	printf("IP identification: %d\n", ip->ip_id);
	printf("IP fragment offset: %d\n", ip->ip_off);
	printf("IP time to live: %d\n", ip->ip_ttl);
	printf("IP protocol: %d\n", ip->ip_p);
	printf("IP checksum: %d\n", ip->ip_sum);
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
	for (size_t i = 0 ; i < sizeof(icmp); ++i)
		printf("%02x ", ((unsigned char *)icmp)[i]);
}

static void print_iovec(t_reply *t)
{
	for (size_t i = 0; i < t->msghdr.msg_iovlen; ++i)
	{
	// 	printf("iovec[%zu].iov_base = %p\n", i, t->msghdr.msg_iov[i].iov_base);
	// 	printf("iovec[%zu].iov_len = %zu\n", i, t->msghdr.msg_iov[i].iov_len);
		// print_ip((struct ip *)t->msghdr.msg_iov[i].iov_base);
		print_icmp((struct icmphdr *)(t->msghdr.msg_iov[i].iov_base + sizeof(struct ip)));
		//print_icmp_hex((struct icmphdr *)(t->msghdr.msg_iov[i].iov_base + sizeof(struct ip)));
	}
}

int ft_ping()
{
	struct timeval t0 = {0, 0};
	struct timeval t = {0, 0};
	ssize_t read;
	struct ping_pkt pckt = {};
	t_reply rply = {};

	ft_bzero(&pckt, sizeof(struct ping_pkt));
	if (!g_state.loop)
		return (0);
	gettimeofday(&t0, NULL);
	build_ping_packet(&pckt, t0);
	read = sendto(g_state.sockfd, (void *)&pckt, sizeof(pckt), 0, (struct sockaddr *)((g_state.addr_list)->ai_addr), sizeof(*(g_state.addr_list)->ai_addr));
	if (read <= 0)
	{
		printf("%s: error: sendto failed\n", BIN);
		return (-1);
	}
	g_state.p_transmitted++;
	
	// while (!rply.received_bytes)
	init_reply(&rply);
	if (g_state.f_opt)
	{
		printf(".");
	}
	if ((rply.received_bytes = recvmsg(g_state.sockfd, &rply.msghdr, 0)) < 0)
	{
		printf("%s: error: recvmsg failed\n", BIN);
		return (-1);
	}
	print_iovec(&rply);
	if (rply.received_bytes < 0 || errno == EAGAIN)
		return (-1);
	// printf("received bytes: %u\n", rply.received_bytes - IP_HDR_LEN);
	if (rply.received_bytes <= 0)
	{
		printf("%s: error: socket closed\n\n", BIN);
		g_state.loop = 0;
		return (-1);
	}
	else if (rply.icmp->icmp_code == ICMP_TIME_EXCEEDED)
	{
		printf("%s: error: time exceeded\n\n", BIN);
		return(-1);
	}
	gettimeofday(&t, NULL);
	g_state.p_received++;

	if (!g_state.f_opt)
	{
		printf("%u bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
			   rply.received_bytes - IP_HDR_LEN, g_state.host, SWAP16(pckt.icmphdr.un.echo.sequence), g_state.ttl, elapsed(t, t0));
	}
	else
		printf("\b");
	return (0);
}