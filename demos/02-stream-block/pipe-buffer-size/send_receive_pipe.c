/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

#include "utils.h"

#define NUM_ROUNDS      5*1024
#define SIZE_TO_WRITE   (1024*1024)

#define diff_us(ta, tb)         \
	(((ta).tv_sec - (tb).tv_sec) * 1000 * 1000 + \
	 ((ta).tv_nsec - (tb).tv_nsec) / 1000)

static const char file_path[] = "file_channel";
static char *buffer;
static unsigned long bufsize;
static int pipefd[2];

static void usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s <bufsize>\n", argv0);
	fprintf(stderr, "    bufsize >= 1, bufsize <= 128K\n");
}

static void parse_bufsize(const char *arg)
{
	char *endp;

	bufsize = strtoul(arg, &endp, 10);
	if (*endp != '\0' || errno == ERANGE || bufsize > 128 * 1024 || bufsize == 0) {
		fprintf(stderr, "Argument is invalid or out of range\n");
		exit(EXIT_FAILURE);
	}
}

static void initialize_buffer(void)
{
	buffer = malloc(bufsize);
	DIE(buffer == NULL, "malloc");
	memset(buffer, 'A', bufsize);
}

static void read_data(void)
{
	int rc;
	unsigned long i, j;
	struct timespec time_before, time_after;

	/* Write data to file. */
	clock_gettime(CLOCK_REALTIME, &time_before);
	for (i = 0; i < NUM_ROUNDS; i++) {
		for (j = 0; j < SIZE_TO_WRITE / bufsize; j++) {
			rc = read(pipefd[0], buffer, bufsize);
			DIE(rc < 0, "write");
		}
	}
	clock_gettime(CLOCK_REALTIME, &time_after);
	printf("reader: time passed %ld microseconds\n", diff_us(time_after, time_before));
}

static void write_data(void)
{
	int rc;
	unsigned long i, j;
	struct timespec time_before, time_after;

	/* Write data to file. */
	clock_gettime(CLOCK_REALTIME, &time_before);
	for (i = 0; i < NUM_ROUNDS; i++) {
		for (j = 0; j < SIZE_TO_WRITE / bufsize; j++) {
			rc = write(pipefd[1], buffer, bufsize);
			DIE(rc < 0, "write");
		}
	}
	clock_gettime(CLOCK_REALTIME, &time_after);
	printf("writer: time passed %ld microseconds\n", diff_us(time_after, time_before));
}

int main(int argc, char **argv)
{
	int rc;
	pid_t pid;

	if (argc != 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	parse_bufsize(argv[1]);
	initialize_buffer();

	rc = pipe(pipefd);
	DIE(rc < 0, "pipe");

	pid = fork();
	switch (pid) {
	case 0: /* child process */
		close(pipefd[0]);
		write_data();
		close(pipefd[1]);
		break;
	case -1: /* error */
		DIE(pid == -1, "fork");
		break;
	default: /* parent process */
		close(pipefd[1]);
		read_data();
		close(pipefd[0]);
		wait(NULL);
		break;
	}

	return 0;
}
