/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.h"

#ifndef BUFSIZ
#define BUFSIZ		256
#endif

unsigned short port = 4242;

static void receive_send(int fd)
{
	static char buffer[BUFSIZ];
	int rc;
	struct sockaddr_in raddr;
	socklen_t raddrlen;

	/* Receive message from client via socket. */
	raddrlen = sizeof(raddr);
	rc = recvfrom(fd, buffer, BUFSIZ, 0, (struct sockaddr *) &raddr, &raddrlen);
	DIE(rc < 0, "recvfrom");

	printf("Received message \"%s\"\n", buffer);
	printf("Sending message back\n");

	/* Send message back to client via socket. */
	rc = sendto(fd, buffer, BUFSIZ, 0, (struct sockaddr *) &raddr, sizeof(raddr));
	DIE(rc < 0, "sendto");
}

int main(void)
{
	int rc;
	int fd;
	struct sockaddr_in addr;
	socklen_t addrlen;
	char buffer[BUFSIZ];

	/* Create socket. */
	fd = socket(PF_INET, SOCK_DGRAM, 0);
	DIE(fd < 0, "socket");

	/* Bind socket to address. */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	rc = bind(fd, (struct sockaddr *) &addr, sizeof(addr));
	DIE(rc < 0, "bind");

	/* Infinite loop: serve clients. */
	while (1) {
		receive_send(fd);
	}

	close(fd);

	return 0;
}
