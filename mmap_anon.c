#include <sys/mman.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *map_with_devzero()
{
	int fd;
	if ((fd = open("/dev/zero", O_RDWR)) == -1) {
		perror("open");
		_exit(EXIT_FAILURE);
	}

	char *addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap devzero");
		_exit(EXIT_FAILURE);
	}

	if (close(fd) == -1) {
		perror("close");
		_exit(EXIT_FAILURE);
	}
	return addr;
}

char *map_anon()
{
	char *addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	if (addr == MAP_FAILED) {
		perror("mmap anon");
		_exit(EXIT_FAILURE);
	}
	return addr;
}

void fork_call(char *addr)
{
	*addr = 1;

	switch(fork()) {
	case -1:
		perror("fork");
		_exit(EXIT_FAILURE);
		break;
	case 0: // child increment the shared integer and exit
		printf("Child started, value = %d\n", *addr);
		(*addr)++;

		if (munmap(addr, sizeof(int)) == -1) {
			perror("munmap");
			_exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	default:
		if (wait(NULL) == -1) {
			perror("wait");
			_exit(EXIT_FAILURE);
		}

		printf("In parent, value = %d\n", *addr);

		if (munmap(addr, sizeof(int)) == -1) {
			perror("munmap");
			_exit(EXIT_FAILURE);
		}
	}
}

// this file shows two ways ot doing a anonymous mmap call in Linux
// one using /dev/zero device file, and another using MAP_ANON mmap
// flag in mmap call
int main(int argc, char *argv[])
{
	fork_call(map_anon());
	fork_call(map_with_devzero());

	return EXIT_SUCCESS;
}
