#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	struct passwd *pwd;
	pwd = getpwnam("marcos");

	if (!pwd) {
		printf("User not found! Aborting\n");
		return EXIT_FAILURE;
	}

	printf("User marcos:\n");
	printf("\tHome dir: %s\n", pwd->pw_dir);
	printf("\tLogin shell: %s\n", pwd->pw_shell);
	printf("\tGroup id: %d\n", pwd->pw_gid);

	uid_t uid = pwd->pw_uid;

	pwd = getpwuid(uid);
	if (!pwd) {
		printf("User not found! Aborting\n");
		return EXIT_FAILURE;
	}

	printf("User marcos(using getpwuid):\n");
	printf("\tHome dir: %s\n", pwd->pw_dir);
	printf("\tLogin shell: %s\n", pwd->pw_shell);
	printf("\tGroup id: %d\n", pwd->pw_gid);

	return EXIT_SUCCESS;
}
