#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "error_aux.h"

int main()
{
	int istack = 222;

	switch (vfork()) {
	case -1:
		exit_failure("vfork");
	case 0: // child executes first in parent's memory space
		setbuf(stdout, NULL);
		printf("Child executing and will sleep for 3 seconds, and the "
			"parent will not be executed until child finishes\n");
		sleep(3);

		write(STDOUT_FILENO, "Child executing\n", 16);
		istack *= 3; // parent will see the change
		_exit(EXIT_SUCCESS);
	default: // parent will be blocked until child exits
		write(STDOUT_FILENO, "Parent executing\n", 17);
		printf("istack = %d\n", istack);
		exit(EXIT_SUCCESS);
	}
}
