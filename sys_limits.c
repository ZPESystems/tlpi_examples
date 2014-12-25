#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sysconfig

enum {
	SC_call,
	PC_call
};

static void confprint(const char* msg, int name, int fd, int type)
{
	errno = 0;

	long lim;
	if (type == SC_call)
		lim = sysconf(name);
	else
		lim = fpathconf(fd, name);

	if (lim != -1) { // ok, limit determinate
		printf("%s: %ld\n", msg, lim);
	} else {
		if (errno == 0) // ok, limit indeterminate
			printf("%s (indeterminate)\n", msg);
		else {
			if (type == SC_call)
				printf("sysconf %d\n", name);
			else
				printf("fpathconfig %d\n", name);
		}
	}
}



int main()
{
	// the _SC_ prefix means SysConf
	confprint("_SC_ARG_MAX:        ", _SC_ARG_MAX, 0, SC_call);
	confprint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX, 0, SC_call);
	confprint("_SC_OPEN_MAX:       ", _SC_OPEN_MAX, 0, SC_call);
	confprint("_SC_NGROUPS_MAX:    ", _SC_NGROUPS_MAX, 0, SC_call);
	confprint("_SC_PAGESIZE:       ", _SC_PAGESIZE, 0, SC_call);
	confprint("_SC_RTSIG_MAX:      ", _SC_RTSIG_MAX, 0, SC_call);

	// the _PC_ prefix means PathConfig
	confprint("_PC_NAME_MAX:       ", _PC_NAME_MAX, STDIN_FILENO, PC_call);
	confprint("_PC_PATH_MAX:       ", _PC_PATH_MAX, STDIN_FILENO, PC_call);
	confprint("_PC_PIPE_BUF:       ", _PC_PIPE_BUF, STDIN_FILENO, PC_call);

	return EXIT_SUCCESS;
}
