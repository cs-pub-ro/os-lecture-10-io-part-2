/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.h"

unsigned short remote_port = 4242;

#define diff_us(ta, tb)         \
	(((ta).tv_sec - (tb).tv_sec) * 1000 * 1000 + \
	 ((ta).tv_nsec - (tb).tv_nsec) / 1000)

static void send_receive(int fd)
{
	static char buffer[BUFSIZ];
	int rc;
	struct timespec time_before, time_after;
	struct sockaddr_in raddr;
	socklen_t raddrlen;

	/* Define remote address. */
	memset(&raddr, 0, sizeof(raddr));
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(remote_port);
	raddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	strcpy(buffer, "gogogo");
	printf("Sending message \"%s\"\n", buffer);

	clock_gettime(CLOCK_REALTIME, &time_before);

	/* Send flag to server via socket. */
	rc = sendto(fd, buffer, BUFSIZ, 0, (struct sockaddr *) &raddr, sizeof(raddr));
	DIE(rc < 0, "sendto");

	/* Read message back from server via socket. */
	raddrlen = sizeof(raddr);
	rc = recvfrom(fd, buffer, BUFSIZ, 0, (struct sockaddr *) &raddr, &raddrlen);
	DIE(rc < 0, "recvfrom");

	clock_gettime(CLOCK_REALTIME, &time_after);

	printf("Received message \"%s\"\n", buffer);
	printf("Round-trip time: %ld microseconds\n", diff_us(time_after, time_before));
}

int main(void)
{
	int rc;
	int fd;

	/* Create socket. */
	fd = socket(PF_INET, SOCK_DGRAM, 0);
	DIE(fd < 0, "socket");

	send_receive(fd);

	close(fd);

	return 0;
}
