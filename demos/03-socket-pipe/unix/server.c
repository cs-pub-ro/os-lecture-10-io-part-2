/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "utils.h"

#ifndef BUFSIZ
#define BUFSIZ		256
#endif

static const char socket_path[] = "socket_channel";

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
	int listenfd, connectfd;
	struct sockaddr_un addr, raddr;
	socklen_t raddrlen;

	/* Remove socket_path. */
	remove(socket_path);

	/* Create socket. */
	listenfd = socket(PF_UNIX, SOCK_STREAM, 0);
	DIE(listenfd < 0, "socket");

	/* Bind socket to path. */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, socket_path);
	rc = bind(listenfd, (struct sockaddr *) &addr, sizeof(addr));
	DIE(rc < 0, "bind");

	/* Put in listen mode. */
	rc = listen(listenfd, 10);
	DIE(rc < 0, "listen");

	/* Accept connection. */
	raddrlen = sizeof(raddr);
	connectfd = accept(listenfd, (struct sockaddr *) &raddr, &raddrlen);
	DIE(connectfd < 0, "accept");

	receive_send(connectfd);

	close(connectfd);
	close(listenfd);

	return 0;
}
