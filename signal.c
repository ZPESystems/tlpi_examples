#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define FILENAME "signalmsg"

static void write_msg(const char* msg);

static void sigHandler(int sig)
{
	write_msg(sig == 10 ? "User defined 1\n" : "User defined 2\n");
}

static void write_msg(const char* msg)
{
	int fd;
	if ((fd = open(FILENAME, O_WRONLY | O_CREAT | O_APPEND,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) >= 0){
		dprintf(fd, msg);
		close(fd);
	}
}

int main(int argc, char** argv)
{
	if (signal(SIGUSR1, sigHandler) == SIG_ERR)
		exit(-1);
	if (signal(SIGUSR2, sigHandler) == SIG_ERR)
		exit(-1);

	int j;
	for (j = 0;; j++)
		sleep(3);
}
