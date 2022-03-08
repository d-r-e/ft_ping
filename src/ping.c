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
	sum += (sum >> 16);
	return ((unsigned short)~sum);
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

int ft_ping()
{
	ssize_t read;
	struct ping_pkt pckt = {};

	ft_bzero(&pckt, sizeof(struct ping_pkt));
	if (!g_state.loop)
		return (0);
	gettimeofday(&g_state.t0, NULL);
	build_ping_packet(&pckt, g_state.t0);
	read = sendto(g_state.sockfd, (void *)&pckt, sizeof(pckt), 0, (struct sockaddr *)((g_state.addr_list)->ai_addr), sizeof(*(g_state.addr_list)->ai_addr));
	if (read <= 0)
	{
		printf("%s: error: sendto failed\n", BIN);
		return (-1);
	}
	g_state.p_transmitted++;
	if (g_state.f_opt)
	{
		printf(".");
	}
	if (g_state.f_opt == 0)
	{
		alarm(1);
	}
	return (0);
}

