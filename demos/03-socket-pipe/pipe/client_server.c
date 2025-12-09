/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils.h"

#ifndef BUFSIZ
#define BUFSIZ		256
#endif

static int pipefd_to_server[2];
static int pipefd_from_server[2];

static void send_receive(void)
{
	static char buffer[BUFSIZ];
	int rc;

	strcpy(buffer, "gogogo");
	printf("Sending message \"%s\"\n", buffer);

	/* Write message to pipe. */
	rc = write(pipefd_to_server[1], buffer, BUFSIZ);
	DIE(rc < 0, "write");

	/* Read message back from pipe. */
	rc = read(pipefd_from_server[0], buffer, BUFSIZ);
	DIE(rc < 0, "read");

	printf("Received message \"%s\"\n", buffer);
}

static void client(void)
{
	close(pipefd_to_server[0]);
	close(pipefd_from_server[1]);

	send_receive();

	close(pipefd_to_server[1]);
	close(pipefd_from_server[0]);
}

static void receive_send(void)
{
	static char buffer[BUFSIZ];
	int rc;

	/* Read message from pipe. */
	rc = read(pipefd_to_server[0], buffer, BUFSIZ);
	DIE(rc < 0, "read");

	printf("Received message \"%s\"\n", buffer);
	printf("Sending message back\n");

	/* Write message back to pipe. */
	rc = write(pipefd_from_server[1], buffer, BUFSIZ);
	DIE(rc < 0, "write");
}

static void server(void)
{
	close(pipefd_to_server[1]);
	close(pipefd_from_server[0]);

	receive_send();

	close(pipefd_to_server[0]);
	close(pipefd_from_server[1]);
}

int main(void)
{
	int rc;
	pid_t pid;

	rc = pipe(pipefd_to_server);
	DIE(rc < 0, "pipe");

	rc = pipe(pipefd_from_server);
	DIE(rc < 0, "pipe");

	pid = fork();
	switch (pid) {
	case 0: /* child process */
		client();
		break;
	case -1: /* error */
		DIE(pid == -1, "fork");
		break;
	default: /* parent process */
		server();
		wait(NULL);
		break;
	}

	return 0;
}
