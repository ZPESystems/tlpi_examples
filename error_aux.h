#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void exit_failure(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
