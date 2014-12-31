#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *f;
	char buf[PATH_MAX];

	f = popen("/bin/ls * 2> /dev/null", "r");
	if (!f) {
		perror("perror");
		return EXIT_FAILURE;
	}

	int fileCnt = 0;
	while (fgets(buf, PATH_MAX, f) != NULL) {
		printf("%s", buf);
		fileCnt++;
	}

	// close pipe
	int status = pclose(f);
	printf("%d matching file%s\n", fileCnt, (fileCnt != 1) ? "s" : "");
	printf("pclose() status == %#x\n", (unsigned int)status);

	exit(EXIT_SUCCESS);
}
