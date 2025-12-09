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

unsigned short remote_port = 4242;

static void send_receive(int fd)
{
	static char buffer[BUFSIZ];
	int rc;

	strcpy(buffer, "gogogo");
	printf("Sending message \"%s\"\n", buffer);

	/* Send message to server via socket. */
	rc = send(fd, buffer, BUFSIZ, 0);
	DIE(rc < 0, "send");

	/* Read message back from server via socket. */
	rc = recv(fd, buffer, BUFSIZ, 0);
	DIE(rc < 0, "recv");

	printf("Received message \"%s\"\n", buffer);
}

int main(void)
{
	int rc;
	int fd;
	int sockfd;

	/* Create socket. */
	fd = socket(PF_INET, SOCK_STREAM, 0);
	DIE(fd < 0, "socket");

	sockfd = tcp_connect_to_server("localhost", remote_port);
	DIE(sockfd < 0, "tcp_connect_to_server");

	send_receive(sockfd);

	close(sockfd);

	return 0;
}
