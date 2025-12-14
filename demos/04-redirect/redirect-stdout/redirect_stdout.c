/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

int main(void)
{
	int fd;

	fd = open("out.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	DIE(fd < 0, "open");

	dup2(fd, 1);
	close(fd);

	puts("Hello, World!");

	return 0;
}
