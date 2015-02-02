#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "error_aux.h"

#define BD_NO_CHDIR		01 // dont chdir /
#define BD_NO_CLOSE_FILES 	02 // doest close all open files
#define BD_NO_REOPEN_STD_FDS 	04 // doesnt map stdin, stdout and stderr to /dev/null

#define BD_NO_UMASK0		10 // dont do umask(0)

#define BD_MAX_CLOSE		8192 // max fd to cloe if sysconf(_SC_OPEN_MAX) is indeterminate

int becameDaemon(int flags)
{
	switch (fork()) { // become background process
	case -1: return -1;
	case 0: break; 	// child continues
	default:  _exit(EXIT_SUCCESS);	// parent dies
	}

	if (setsid() == -1)
		exit_failure("setsid");

	switch (fork()) { // ensure we're not the session leader
	case -1: return -1;
	case 0: break;
	default:  _exit(EXIT_SUCCESS);
	}

	if (!(flags & BD_NO_UMASK0)) // clear file mode creation mask
		umask(0);

	if (!(flags & BD_NO_CHDIR)) // changes to root directory
		chdir("/");

	int fd;
	if (!(flags & BD_NO_CLOSE_FILES)) { // close all open files
		int maxfd = sysconf(_SC_OPEN_MAX);
		if (maxfd == -1) // indeterminate, so take a guess
			maxfd = BD_MAX_CLOSE;

		for (fd = 0; fd < maxfd; fd++)
			close(fd);
	}

	if (!(flags & BD_NO_REOPEN_STD_FDS)) { // map all std* open files to /dev/null
		close(STDIN_FILENO);

		fd = open("/dev/null", O_RDWR);

		if (fd != STDIN_FILENO) // we must ensure fd now is 0
			return -1;
		if (dup2(STDIN_FILENO, STDOUT_FILENO) == -1)
			return -1;
		if (dup2(STDIN_FILENO, STDERR_FILENO) == -1)
			return -1;
	}

	return 0;
}

int main()
{
	becameDaemon(0);
	sleep(30); // just to verify the ps output
	return 0;

}
