#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

int main()
{
	int fd, flags;
	char template[] = "/tmp/testXXXXXX";

	setbuf(stdout, NULL); // disable buffer to stdout

	if ((fd = mkstemp(template)) == -1) {
		perror("mkstemp");
		_exit(EXIT_FAILURE);
	}
	
	printf("File offset before fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

	if ((flags = fcntl(fd, F_GETFL)) == -1) {
		perror("fcntl - F_GETFL");
		_exit(EXIT_FAILURE);
	}

	printf("Append flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

	switch (fork()) {
	case -1:
		perror("fork");
		return EXIT_FAILURE;
	case 0: // Child: change file offset and status flag
		if (lseek(fd, 1000, SEEK_SET) == -1) {
			perror("lseek");
			_exit(EXIT_FAILURE);
		}

		if ((flags = fcntl(fd, F_GETFL)) == -1) {
			perror("fcntl - F_GETFL");
			_exit(EXIT_FAILURE);
		}

		flags |= O_APPEND;
		if (fcntl(fd, F_SETFL, flags) == -1) {
			perror("fcntl - F_SETFL");
			_exit(EXIT_FAILURE);
		}

		_exit(EXIT_SUCCESS);

	default: // parent: see file changes made by child
		if (wait(NULL) == -1) {
			perror("wait");
			_exit(EXIT_FAILURE);
		}

		printf("Child has exited\n");
		printf("File offset in parent: %lld\n",
						(long long) lseek(fd, 0, SEEK_CUR));

		if ((flags = fcntl(fd, F_GETFL)) == -1) {
			perror("fcntl");
			_exit(EXIT_FAILURE);
		}

		printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
		exit(EXIT_SUCCESS);
	}
}
