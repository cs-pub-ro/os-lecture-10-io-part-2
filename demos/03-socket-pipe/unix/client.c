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

static const char socket_path[] = "socket_channel";

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
	struct sockaddr_un addr;

	/* UNIX socket must exist. It must be created by the server. */
	rc = access(socket_path, R_OK | W_OK);
	DIE(rc < 0, "access");

	/* Open socket. */
	fd = socket(PF_UNIX, SOCK_STREAM, 0);
	DIE(fd < 0, "open");

	/* Connect socket to server. */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, sizeof(socket_path), "%s", socket_path);
	rc = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
	DIE(rc < 0, "connect");

	send_receive(fd);

	close(fd);

	return 0;
}
