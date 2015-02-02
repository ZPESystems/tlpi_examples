#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "error_aux.h"

int main(int argc, char *argv[])
{
	int fd;
	off64_t off;

	if (argc != 3) {
		printf("%s pathname offset\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
		exit_failure("open64");

	off = atoll(argv[2]);
	if (lseek64(fd, off, SEEK_SET) == -1)
		exit_failure("lseek64");

	if (write(fd, "test", 4) == -1)
		exit_failure("write");

	exit(EXIT_SUCCESS);
}
