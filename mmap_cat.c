#include <sys/mman.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "error_aux.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Need one argument. Aborting\n");
		exit(EXIT_FAILURE);
	}

	char *addr;
	int fd;
	struct stat st;

	if ((fd = open(argv[1], O_RDONLY)) == -1)
		exit_failure("open");

	// get size of file to use as size of mapping
	if (fstat(fd, &st) == -1)
		exit_failure("fstat");

	// the last parameter is the file offset. Zero here means to map the entire file
	// into memory
	if ((addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		exit_failure("mmap");

	// read from mmaped file
	if (write(STDOUT_FILENO, addr, st.st_size) != st.st_size) {
		printf("write");
		exit(EXIT_FAILURE);
	}

	if (munmap(addr, st.st_size) == -1)
		exit_failure("munmap");

	return EXIT_SUCCESS;
}
