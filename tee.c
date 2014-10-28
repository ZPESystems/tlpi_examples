#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int fd;
	size_t len;
	ssize_t nread;
	char *filepath = argv[1];
	mode_t mode = O_WRONLY | O_CREAT;

	if (argc < 2) {
		printf("argc: needs a file as arg\n");
		exit(EXIT_FAILURE);
	}

	int opt = getopt(argc, argv, "a:");
	if (opt == 'a') {
		mode |= O_APPEND;
		filepath = optarg;
	} else {
		mode |= O_TRUNC;
	}

	fd = open(filepath, mode, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	char c[BUF_SIZE];
	len = sizeof(c);
	while ((nread = read(STDIN_FILENO, c, len)) > 0) {
		printf("%s", c);
		if (write(fd, c, nread) != nread) {
			perror("write less than len");
			exit(EXIT_FAILURE);
		}
	}

	if (nread == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	if (close(fd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
