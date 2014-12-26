#include <dirent.h>
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

uid_t getUid(const char *username)
{
	struct passwd *pwd;

	pwd = getpwnam(username);
	if (!pwd)
		return -1;
	return pwd->pw_uid;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("You need to inform an username. Aborting\n");
		return EXIT_FAILURE;
	}

	uid_t uid;
	if ((uid = getUid(argv[1])) == -1) {
		printf("Username %s not found. Aborting\n", argv[1]);
		return EXIT_FAILURE;
	}

	DIR *dir = opendir("/proc/");
	if (!dir) {
		perror("opendir");
		return EXIT_FAILURE;
	}

	struct dirent *ep;

	while ((ep = readdir(dir))) {
		char pathname[255];
		sprintf(pathname, "/proc/%s/status", ep->d_name);

		FILE *f = fopen(pathname, "r");
		if (!f) {
			// not a process dir, or processes already closed
			continue;
		}

		ssize_t read;
		size_t len = 0;
		char *line = NULL;

		char pname[255];
		int pid, tmpuid;

		while((read = getline(&line, &len, f)) != -1) {
			if (!strncmp(line, "Name:", 5))
				sscanf(line, "Name:\t%s", pname);
			else if (!strncmp(line, "Pid:", 4))
				sscanf(line, "Pid:\t%d", &pid);
			else if (!strncmp(line, "Uid:", 4))
				sscanf(line, "Uid:\t%d", &tmpuid);
		}

		free(line);

		// this process is not from the user we want
		if (tmpuid != uid)
			continue;

		printf("Pid: %d, process name: %s\n", pid, pname);
	}

	closedir(dir);

	return EXIT_SUCCESS;
}
