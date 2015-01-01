#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// max number of bytes writen to the file
#define MEM_SIZE 10

int main(int argc, char *argv[])
{
	char *addr;
	int fd;

	if (argc < 2) {
		fprintf(stderr, "Must have at least one parameter of a file path. Aborting\n");
		_exit(EXIT_FAILURE);
	}

	if ((fd = open(argv[1], O_RDWR)) == -1) {
		perror("open");
		_exit(EXIT_FAILURE);
	}

	if ((addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap");
		_exit(EXIT_FAILURE);
	}

	// we dont need the fd anymore
	if (close(fd) == -1) {
		perror("mmap");
		_exit(EXIT_FAILURE);
	}

	// secure practice: output at most MEM_SIZE bytes
	printf("Current content of file=%.*s\n", MEM_SIZE, addr);

	if (argc > 2) {
		if (strlen(argv[2]) >= MEM_SIZE) {
			fprintf(stderr, "new value is too large");
			_exit(EXIT_FAILURE);
		}

		memset(addr, 0, MEM_SIZE); // zero out region
		strncpy(addr, argv[2], MEM_SIZE - 1);
		if (msync(addr, MEM_SIZE, MS_SYNC) == -1) {
			perror("msync");
			_exit(EXIT_FAILURE);
		}

		printf("Copied \"%s\" to shared memory\n", argv[2]);
	}

	if (munmap(addr, MEM_SIZE) == -1) {
		perror("munmap");
		_exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
