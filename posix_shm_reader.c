#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Need an SHM name. Aborting\n");
		_exit(EXIT_FAILURE);
	}

	// open existent shm object
	int fd = shm_open(argv[1], O_RDONLY, 0);
	if (fd == -1) {
		perror("shm_open");
		_exit(EXIT_FAILURE);
	}

	struct stat sb;
	// get memory object size
	if (fstat(fd, &sb) == -1) {
		perror("fstat");
		_exit(EXIT_FAILURE);
	}

	char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		_exit(EXIT_FAILURE);
	}

	if (close(fd) == -1) {
		perror("close");
		_exit(EXIT_FAILURE);
	}

	write(STDOUT_FILENO, addr, sb.st_size);
	printf("\n");

	if (shm_unlink(argv[1]) == -1) {
		perror("shm_unlink");
		_exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
