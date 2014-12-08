#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	off64_t off;

	if (argc != 3) {
		printf("%s pathname offset\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("open64");
		exit(EXIT_FAILURE);
	}
	off = atoll(argv[2]);
	if (lseek64(fd, off, SEEK_SET) == -1) {
		perror("lseek64");
		exit(EXIT_FAILURE);
	}
	if (write(fd, "test", 4) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
