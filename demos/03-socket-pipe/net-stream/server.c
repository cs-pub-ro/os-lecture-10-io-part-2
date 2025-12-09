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
#include "sock/sock_util.h"

#ifndef BUFSIZ
#define BUFSIZ		256
#endif

#define LISTEN_BACKLOG	10

unsigned short port = 4242;

static void receive_send(int fd)
{
	static char buffer[BUFSIZ];
	int rc;

	/* Receive message from client via socket. */
	rc = recv(fd, buffer, BUFSIZ, 0);
	DIE(rc < 0, "recv");

	printf("Received message \"%s\"\n", buffer);
	printf("Sending message back\n");

	/* Send message to client via socket. */
	rc = send(fd, buffer, BUFSIZ, 0);
	DIE(rc < 0, "send");
}

int main(void)
{
	int rc;
	int listenfd, sockfd;
	struct sockaddr_in raddr;
	socklen_t raddrlen;

	/* Create socket. */
	listenfd = tcp_create_listener(port, LISTEN_BACKLOG);
	DIE(listenfd < 0, "tpc_create_listener");

	/* Accept connection. */
	raddrlen = sizeof(raddr);
	sockfd = accept(listenfd, (SSA *) &raddr, &raddrlen);
	DIE(sockfd < 0, "accept");

	receive_send(sockfd);

	close(sockfd);

	return 0;
}
