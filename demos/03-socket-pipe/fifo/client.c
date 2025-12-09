/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "utils.h"

static const char to_server_path[] = "to_server.fifo";
static const char from_server_path[] = "from_server.fifo";

#ifndef BUFSIZ
#define BUFSIZ		256
#endif

int main(void)
{
	int rc;
	int fd1, fd2;
	char buffer[BUFSIZ];

	/*
	 * Create FIFO if it doesn't exist or if it exists and has incorrect
	 * permissions.
	 */
	rc = access(from_server_path, R_OK | W_OK);
	if (rc < 0) {
		rc = mkfifo(from_server_path, 0755);
		DIE(rc < 0, "mkfifo");
	}

	strcpy(buffer, "gogogo");
	printf("Sending message \"%s\"\n", buffer);

	/* Open FIFO */
	fd1 = open(to_server_path, O_WRONLY);
	DIE(fd1 < 0, "open");

	/* Write message back to FIFO. */
	rc = write(fd1, buffer, BUFSIZ);
	DIE(rc < 0, "write");

	/* Open FIFO. */
	fd2 = open(from_server_path, O_RDONLY);
	DIE(fd2 < 0, "open");

	/* Read message back from FIFO. */
	rc = read(fd2, buffer, BUFSIZ);
	DIE(rc < 0, "read");

	printf("Received message \"%s\"\n", buffer);

	close(fd1);
	close(fd2);

	return 0;
}
