#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>

#define MOD_PATH "/dev/course_module"

void handle_read(void)
{
	char tmp[10];
	char tmp2[100];
	char tmp3[100];
	int fd, ret;

	fd = open(MOD_PATH, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	memset(&tmp, 0, sizeof(tmp));

	ret = read(fd, tmp, sizeof(tmp) - 1);
	if (ret == -1) {
		perror("read");
		exit(1);
	}
	printf("%s\n", tmp);

	memset(&tmp2, 0, sizeof(tmp2));
	ret = read(fd, tmp2, sizeof(tmp2) -1);
	if (ret == -1) {
		perror("read2");
		exit(1);
	}
	printf("%s\n", tmp2);

	close(fd);

	memset(&tmp2, 0, sizeof(tmp2));
	// skip error checking
	fd = open("/dev/course_module", O_RDONLY);
	ret = read(fd, tmp3, sizeof(tmp3) - 1);
	printf("%s\n", tmp3);
	close(fd);
}

static void handle_write(char *str)
{
	int ret, fd;
	char tmp[14];
	strcpy(tmp, str);

	fd = open(MOD_PATH, O_WRONLY);
	if (fd == -1) {
		perror("write");
		exit(1);
	}

	ret = write(fd, tmp, sizeof(tmp));
	if (ret == -1) {
		perror("write");
		exit(1);
	}

	close(fd);
}

void usage(const char *prog_name)
{
	printf("Usage: %s <-r | -w some_string>\n", prog_name);
	exit(1);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		usage(argv[0]);

	if (!strncmp(argv[1], "r", 1)) {
		handle_read();
	} else if (!strncmp(argv[1], "w", 1)) {
		if (argc !=3)
			usage(argv[0]);

		handle_write(argv[2]);
	} else {
		usage(argv[0]);
	}

	return 0;
}
