#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error_aux.h"

int main()
{
	int fds[2];

	if (pipe(fds) == -1)
		exit_failure("pipe");

	switch (fork()) {
	case -1:
		exit_failure("fork1");

	case 0: // first child: exec ls to write to pipe
		if (close(fds[0]) == -1)
			exit_failure("close 1");

		// Duplicate stdout on write end of pipe and close duplicated descriptor
		if (fds[1] != STDOUT_FILENO) { // defensive check to verify is the write pipe wasn't reset before
			if (dup2(fds[1], STDOUT_FILENO) == -1)
				exit_failure("dup2 1");

			if (close(fds[1]) == -1)
				exit_failure("close 2");
		}

		execlp("ls", "ls", (char *)NULL); // this will write the output of ls command into the write end of pipe
		return EXIT_FAILURE; // SHOULD NOT HIT HERE!
	default: // parent, needs to creat next child
		break;
	}

	switch (fork()) {
	case -1:
		exit_failure("fork2");

	case 0: // second child: exec wc to read from pipe
		if (close(fds[1]) == -1) // close write pipe
			exit_failure("close 3");

		// Duplicate stdin on read end of pipe and close duplicated descriptor
		if (fds[0] != STDIN_FILENO) { // defensive check to verify is the read pipe wasn't reset before
			if (dup2(fds[0], STDIN_FILENO) == -1)
				exit_failure("dup2 2");

			if (close(fds[0]) == -1)
				exit_failure("close 4");
		}

		execlp("wc", "wc", "-l", (char *)NULL); // read the pipe, full 
		return EXIT_FAILURE; // SHOULD NOT HIT HERE!
	default: // parent, doest nothing
		break;
	}

	// parent closes all unused fds for pipe and waits for children
	if (close(fds[0]) == -1)
		exit_failure("close 5");

	if (close(fds[1]) == -1)
		exit_failure("close 6");

	if (wait(NULL) == -1)
		exit_failure("wait 1");

	if (wait(NULL) == -1)
		exit_failure("wait 2");

	return EXIT_SUCCESS;
}
