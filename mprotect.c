#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>

#define LEN (1024 * 1024)
#define SHELL_FMT "cat /proc/%ld/maps | grep zero"

int main()
{
	char *addr;
	char cmd[255];

	if ((addr = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
		perror("mmap");
		_exit(EXIT_FAILURE);
	}

	snprintf(cmd, 255, SHELL_FMT, (long)getpid());

	// display line from /proc/self/maps corresponding to mapping
	printf("Before mprotect()\n");
	system(cmd);

	if (mprotect(addr, LEN, PROT_READ | PROT_WRITE) == -1) {
		perror("mprotect");
		_exit(EXIT_FAILURE);
	}

	// show maps again after the mprotect call
	printf("After mprotect()\n");
	system(cmd);

	if (munmap(addr, LEN) == -1) {
		perror("munmap");
		_exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
