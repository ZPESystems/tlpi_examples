#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	int flags = O_RDWR | O_CREAT | O_EXCL;
	mode_t perms = S_IRUSR | S_IWUSR;
	size_t len;
	void *addr;

	if (argc < 3) {
		printf("Need a name and data as arguments.Aborting\n");
		_exit(EXIT_FAILURE);
	}

	int fd = shm_open(argv[1], flags, perms);
	if (fd == -1) {
		perror("shm_open");
		_exit(EXIT_FAILURE);
	}

	len = strlen(argv[2]);
	if (ftruncate(fd, len) == -1) {
		perror("ftruncate");
		_exit(EXIT_FAILURE);
	}

	if ((addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap");
		_exit(EXIT_FAILURE);
	}

	// fd not longer needed
	if (close(fd) == -1) {
		perror("close");
		_exit(EXIT_FAILURE);
	}

	printf("Copying %ld bytes\n", (long)len);
	memcpy(addr, argv[2], len); // copying to shared memory

	exit(EXIT_SUCCESS);
}
