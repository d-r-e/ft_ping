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

static
unsigned short checksum(void *address, size_t len)
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

// static void print_rply_hex(t_reply *rply)
// {
// 	int i;
// 	unsigned int width = 10;

// 	i = 0;
// 	while (i < rply->received_bytes)
// 	{
// 		printf("%02x ", ((char*)rply->iov.iov_base)[i]);
// 		if (i % width == width - 1)
// 			printf("\n");
// 		i++;
// 	}
// 	printf("\n");
// }


static void init_reply(t_reply *rply)
{
	rply->msghdr.msg_name = NULL;
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
// 	size_t width = 16;
// 	i = 0;
// 	while (i < sizeof(struct ping_pkt))
// 	{
// 		if (i % width == 0)
// 			printf("000%02lx: ", i);
// 		printf("%02x ", ((unsigned char *)pkt)[i]);
// 		if (i % width == width - 1)
// 			printf("\n");
// 		i++;
// 	}
// 	printf("\n");
// }

/*
 *  Reads from /sys/class/net/<iface>/statistics/<stat>
 */
void read_mac_address(void *mem)
{
	int fd;

	fd = open("/sys/class/net/eth0/address", O_RDONLY);
	read(fd, mem, 17);
	close(fd);
}

// static void print_packet_fields(struct ping_pkt pkt)
// {
// 	printf("\n");
// 	printf("\tPacket size: %ld\n", sizeof(pkt));
// 	printf("\tPacket type: %d\n", pkt.icmphdr.type);
// 	printf("\tPacket code: %d\n", pkt.icmphdr.code);
// 	printf("\tPacket checksum: %d\n", pkt.icmphdr.checksum);
// 	printf("\tPacket id: %d\n", SWAP16(pkt.icmphdr.un.echo.id));
// 	printf("\tPacket sequence: %d\n", SWAP16(pkt.icmphdr.un.echo.sequence));

// 	printf("\n");
// }

static void build_ping_packet(struct ping_pkt *packet, struct timeval current_time)
{
	static int msg_count = 0;

	ft_bzero(packet, sizeof(packet));
	packet->icmphdr.type = ICMP_ECHO;
	packet->icmphdr.code = 0;
	packet->icmphdr.un.echo.id = SWAP16(getpid());
	packet->icmphdr.un.echo.sequence = SWAP16(msg_count);
		msg_count++;
	ft_memcpy(&packet->icmphdr.un, &(current_time.tv_sec), sizeof(current_time.tv_sec));
	packet->icmphdr.checksum = (checksum(packet, sizeof(packet)));
	// print_packet_fields(*packet);
	// print_hex_packet(packet);
}

int ft_ping()
{
	struct timeval t0 = {0, 0};
	struct timeval t = {0, 0};
	ssize_t read;
	struct ping_pkt pckt = {};
	t_reply rply = {};

	ft_bzero(&pckt, sizeof(pckt));
	if (!g_state.loop)
		return (0);
	gettimeofday(&t0, NULL);
	build_ping_packet(&pckt, t0);
	read = sendto(g_state.sockfd, (void *)&pckt, sizeof(pckt), 0, (struct sockaddr *)g_state.addr_list->ai_addr, sizeof(*g_state.addr_list->ai_addr));
	if (read <= 0)
	{
		printf("%s: error: sendto failed\n", BIN);
		return (-1);
	}
	g_state.p_transmitted++;
	ft_bzero(&rply, sizeof(rply));
	// while (!rply.received_bytes)
	init_reply(&rply);
	if (g_state.f_opt)
	{
		printf(".");
	}
	rply.received_bytes = recvmsg(g_state.sockfd, &(rply.msghdr), 0);
	if (rply.received_bytes < 0)
		return (-1);
		// printf("%s: error: recvfrom failed\n", BIN);
	if (rply.received_bytes < PING_SZ)
	{
		printf("%s: error: socket closed\n\n", BIN);
		g_state.loop = 0;
		return (-1);
	}
	gettimeofday(&t, NULL);
	// for (size_t i = 0; i < rply.msghdr.msg_iovlen; ++i)
	// 	printf("%.*s\n", (int)rply.iov.iov_len, (const char*)rply.msghdr.msg_iov[i].iov_base);
	// // printf("%.*s\n", 24, (const char*)rply.control);
	printf("reply code %d\n", (unsigned char)((struct icmphdr *)(rply.receive_buffer + sizeof(struct icmphdr)))->code);
	printf("reply type %d\n", (unsigned char)((struct icmphdr *)(rply.receive_buffer + sizeof(struct icmphdr)))->type);

	g_state.p_received++;

	if (!g_state.f_opt)
	{
		printf("%u bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
			   rply.received_bytes - 12, g_state.host, SWAP16(pckt.icmphdr.un.echo.sequence), g_state.ttl, elapsed(t, t0));
	}
	else
		printf("\b");
	return (0);
}