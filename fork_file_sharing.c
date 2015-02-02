#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

#include "error_aux.h"

int main()
{
	int fd, flags;
	char template[] = "/tmp/testXXXXXX";

	setbuf(stdout, NULL); // disable buffer to stdout

	if ((fd = mkstemp(template)) == -1)
		exit_failure("mkstemp");
	
	printf("File offset before fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

	if ((flags = fcntl(fd, F_GETFL)) == -1)
		exit_failure("fcntl - F_GETFL");

	printf("Append flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

	switch (fork()) {
	case -1:
		exit_failure("fork");
	case 0: // Child: change file offset and status flag
		if (lseek(fd, 1000, SEEK_SET) == -1)
			exit_failure("lseek");

		if ((flags = fcntl(fd, F_GETFL)) == -1)
			exit_failure("fcntl - F_GETFL 2");

		flags |= O_APPEND;
		if (fcntl(fd, F_SETFL, flags) == -1)
			exit_failure("fcntl - F_SETFL");

		_exit(EXIT_SUCCESS);

	default: // parent: see file changes made by child
		if (wait(NULL) == -1)
			exit_failure("wait");

		printf("Child has exited\n");
		printf("File offset in parent: %lld\n",
						(long long) lseek(fd, 0, SEEK_CUR));

		if ((flags = fcntl(fd, F_GETFL)) == -1)
			exit_failure("fcntl - F_GETFL 3");

		printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
		exit(EXIT_SUCCESS);
	}
}
