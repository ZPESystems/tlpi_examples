#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error_aux.h"
#include "fifo.h"

static char clientFifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo(void)
{
	unlink(clientFifo);
}

int main(int argc, char *argv[])
{
	int sfd, cfd;
	struct request req;
	struct response resp;

	req.pid = getpid();

	if (argc > 1)
		req.seqLen = atoi(argv[1]);
	else
		req.seqLen = 1;

	umask(0);
	snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long) getpid());
	if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1)
		exit_failure("mkfifo");

	if (atexit(removeFifo) == -1)
		exit_failure("atexit");

	sfd = open(SERVER_FIFO, O_WRONLY);
	if (sfd == -1)
		exit_failure("open 1");

	if (write(sfd, &req, sizeof(struct request)) != sizeof(struct request)) {
		printf("write was not successful\n");
		return EXIT_FAILURE;
	}

	// open fifo, read and display data
	cfd = open(clientFifo, O_RDONLY);
	if (cfd == -1)
		exit_failure("open 2");

	if (read(cfd, &resp, sizeof(struct response)) != sizeof(struct response)) {
		printf("read error\n");
		return EXIT_FAILURE;
	}

	printf("%d\n", resp.seqNum);
	return EXIT_SUCCESS;
}
