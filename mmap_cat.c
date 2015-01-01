#include <sys/mman.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Need one argument. Aborting\n");
		_exit(EXIT_FAILURE);
	}

	char *addr;
	int fd;
	struct stat st;

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror("open");
		_exit(EXIT_FAILURE);
	}

	// get size of file to use as size of mapping
	if (fstat(fd, &st) == -1) {
		perror("fstat");
		_exit(EXIT_FAILURE);
	}

	// the last parameter is the file offset. Zero here means to map the entire file
	// into memory
	if ((addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		perror("mmap");
		_exit(EXIT_FAILURE);
	}

	// read from mmaped file
	if (write(STDOUT_FILENO, addr, st.st_size) != st.st_size) {
		perror("write");
		_exit(EXIT_FAILURE);
	}

	if (munmap(addr, st.st_size) == -1) {
		perror("munmap");
		_exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
