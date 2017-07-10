#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/fs.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	unsigned long blocks = 0;
	int fd;

	if (argc == 1) {
		printf("Usage: %s <path to disk>\n", argv[0]);
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	if (ioctl(fd, BLKGETSIZE, &blocks)) {
		perror("ioctl");
		exit(1);
	}

	close(fd);

	printf("blocks %lu, size %.3f in GB\n"
			, blocks
			, (double)blocks * 512.0 / (1024 * 1024 * 1024));
}
