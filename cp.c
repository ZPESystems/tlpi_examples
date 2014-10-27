#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int fdin, fdout, flags;
	mode_t file_perm;
	ssize_t nread;
	char buf[BUF_SIZE];

	if (argc != 3) {
		printf("%s", "old new\n");
		exit(EXIT_FAILURE);
	}

	fdin = open(argv[1], O_RDONLY);	
	if (fdin == -1) {
		perror("open fdin");
		exit(EXIT_FAILURE);
	}

	flags = O_CREAT | O_WRONLY | O_TRUNC;
	file_perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	fdout = open(argv[2], flags, file_perm);
	if (fdout == -1) {
		perror("open fdout");
		exit(EXIT_FAILURE);
	}

	while ((nread = read(fdin, buf, BUF_SIZE)) > 0)
		if (write(fdout, buf, nread) != nread) {
			perror("couldn't write the whole buffer");
			exit(EXIT_FAILURE);
		}

	if (nread == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	if (close(fdin) == -1) {
		perror("close fdin");
		exit(EXIT_FAILURE);
	}
	if (close(fdout) == -1) {
		perror("close fdin");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
