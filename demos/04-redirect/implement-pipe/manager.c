/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils.h"

int main(void)
{
	int rc;
	int pipefd[2];
	pid_t pid1, pid2;

	rc = pipe(pipefd);
	DIE(rc < 0, "pipe");

	pid1 = fork();
	switch (pid1) {
	case -1:
		DIE(pid1 == -1, "fork");
		break;
	case 0:
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		rc = execl("./reader", "./reader", NULL);
		DIE(rc < 0, "execl");
	default:
		break;
	}

	pid2 = fork();
	switch (pid2) {
	case -1:
		DIE(pid2 == -1, "fork");
		break;
	case 0:
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		rc = execl("./writer", "./writer", NULL);
		DIE(rc < 0, "execl");
	default:
		break;
	}

	close(pipefd[0]);
	close(pipefd[1]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	return 0;
}
