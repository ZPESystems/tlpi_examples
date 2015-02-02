#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include "error_aux.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Need an SHM name. Aborting\n");
		exit(EXIT_FAILURE);
	}

	// open existent shm object
	int fd = shm_open(argv[1], O_RDONLY, 0);
	if (fd == -1)
		exit_failure("shm_open");

	struct stat sb;
	// get memory object size
	if (fstat(fd, &sb) == -1)
		exit_failure("fstat");

	char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
		exit_failure("mmap");

	if (close(fd) == -1)
		exit_failure("close");

	write(STDOUT_FILENO, addr, sb.st_size);
	printf("\n");

	if (shm_unlink(argv[1]) == -1)
		exit_failure("shm_unlink");

	exit(EXIT_SUCCESS);
}
