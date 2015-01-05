#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	int flags = O_RDWR | O_CREAT | O_EXCL;
	mode_t perms = S_IRUSR | S_IWUSR;
	size_t size;
	void *addr;

	if (argc < 3) {
		printf("Need a name and size.Aborting\n");
		_exit(EXIT_FAILURE);
	}

	size = (size_t)atoi(argv[2]);

	int fd;
	if ((fd = shm_open(argv[1], flags, perms)) == -1) {
		perror("shm_open");
		_exit(EXIT_FAILURE);
	}

	if (ftruncate(fd, size) == -1) {
		perror("ftruncate");
		_exit(EXIT_FAILURE);
	}

	// does nothing, just create the shm and mmap
	if ((addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap");
		_exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
