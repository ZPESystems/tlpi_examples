#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#include "error_aux.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("path needed\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(argv[1], O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1)
		exit_failure("open");

	if (lseek(fd, 0, SEEK_SET) == -1)
		exit_failure("lseek");

	if (write(fd, "a", sizeof(char)) == -1)
		exit_failure("write");

	if (close(fd) == -1)
		exit_failure("close");

	exit(EXIT_SUCCESS);
}
