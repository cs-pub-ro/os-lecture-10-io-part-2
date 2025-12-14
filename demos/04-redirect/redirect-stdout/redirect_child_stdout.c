/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "utils.h"

int main(void)
{
	int fd;
	pid_t pid;
	int rc;

	fd = open("out.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	DIE(fd < 0, "open");

	pid = fork();
	switch (pid) {
	case -1:
		DIE(pid == -1, "fork");
		break;
	case 0:
		dup2(fd, 1);
		rc = execl("/bin/ls", "/bin/ls", NULL);
		DIE(rc < 0, "execl");
	default:
		break;
	}

	close(fd);
	waitpid(pid, NULL, 0);

	return 0;
}
