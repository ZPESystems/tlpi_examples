#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>

#define LEN (1024 * 1024)
#define SHELL_FMT "cat /proc/%ld/maps | grep zero"

#include "error_aux.h"

int main()
{
	char *addr;
	char cmd[255];

	if ((addr = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		exit_failure("mmap");

	snprintf(cmd, 255, SHELL_FMT, (long)getpid());

	// display line from /proc/self/maps corresponding to mapping
	printf("Before mprotect()\n");
	system(cmd);

	if (mprotect(addr, LEN, PROT_READ | PROT_WRITE) == -1)
		exit_failure("mprotect");

	// show maps again after the mprotect call
	printf("After mprotect()\n");
	system(cmd);

	if (munmap(addr, LEN) == -1)
		exit_failure("munmap");

	exit(EXIT_SUCCESS);
}
