#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int idata = 111; // allocated in data segment

int main(int argc, char *argv[])
{
	int istack = 222; // allocaged in stack segment
	pid_t childpid;

	switch(childpid = fork()) {
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	case 0:
		idata *= 3;
		istack *= 3;
		break;
	default:
		sleep(3);
		break;
	}

	// parent and chid came here
	printf("PID=%ld %s idata=%d istack=%d\n", (long)getpid(),
			(childpid == 0) ? "(child)" : "(parent)", idata, istack);

	exit(EXIT_SUCCESS);
}
