#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_aux.h"

int main()
{
	// fd 0 == read side
	// fd 1 write side
	int fds[2];
	char buf[255];

	if (pipe(fds) == -1)
		exit_failure("pipe");

	switch(fork()) {
	case -1:
		exit_failure("fork");

	case 0: // child
		if (close(fds[1]) == -1) // closes the write fd as it will not be used
			exit_failure("Child close");

		if (read(fds[0], buf, sizeof(buf)) == -1)
			exit_failure("Child read");

		printf("Child readed: %s\n", buf);
		break;

	default: // parent
		if (close(fds[0]) == -1) // closes the read fd as it will not be used
			exit_failure("Child close");

		strncpy(buf, "Hello from parent! Have a long life!", sizeof(buf));
		if (write(fds[1], buf, sizeof(buf)) == -1)
			exit_failure("Parent write");

		printf("Parent wrote the message!\n");
		break;
	}

	return EXIT_SUCCESS;
}
