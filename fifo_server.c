#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "error_aux.h"
#include "fifo.h"

int main()
{
	int sfd, cfd, dfd;
	char clientFifo[CLIENT_FIFO_NAME_LEN];
	struct request req;
	struct response resp;
	int seqNum = 0; // server service

	umask(0);
	if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1)
		exit_failure("mkfifo");

	sfd = open(SERVER_FIFO, O_RDONLY);
	if (sfd == -1)
		exit_failure("open");

	// open dummy fd, thus the server will never receive EOF
	dfd = open(SERVER_FIFO, O_WRONLY);
	if (dfd == -1)
		exit_failure("open 2");

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		exit_failure("signal");

	for (;;) {
		if (read(sfd, &req, sizeof(struct request)) != sizeof(struct request)) {
			printf("Error while reading, discarding");
			continue;
		}

		// open fifo previously created by client
		snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)req.pid);
		cfd = open(clientFifo, O_WRONLY);
		if (cfd == -1) {
			printf("open 3"); // open failed, ignore client
			continue;
		}

		resp.seqNum = seqNum;
		if (write(cfd, &resp, sizeof(struct response)) != sizeof(struct response))
			printf("error writing to fifo %s\n", clientFifo);

		if (close(cfd) == -1)
			exit_failure("close");

		seqNum += req.seqLen;
	}

	return EXIT_FAILURE;
}
