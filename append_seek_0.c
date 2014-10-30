#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("path needed\n");
		exit(EXIT_FAILURE);
	}
	int fd = open(argv[1], O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("lseek");
		exit(EXIT_FAILURE);
	}

	if (write(fd, "a", sizeof(char)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}

	if (close(fd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
