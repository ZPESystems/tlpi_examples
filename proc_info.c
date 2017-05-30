#define _GNU_SOURCE

#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>

#include "error_aux.h"

int main()
{
	struct utsname uts;

	if (uname(&uts) == -1)
		exit_failure("stat");

	printf("Node name:   %s\n", uts.nodename);
	printf("System name: %s\n", uts.sysname);
	printf("Release:     %s\n", uts.release);
	printf("Version:     %s\n", uts.version);
	printf("Machine:     %s\n", uts.machine);
#if defined(_GNU_SOURCE) && defined(__linux__)
	printf("Domain name: %s\n", uts.domainname);
#endif

	exit(EXIT_SUCCESS);
}
