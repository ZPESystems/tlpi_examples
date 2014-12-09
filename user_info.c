#include <pwd.h>
#include <grp.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

	printf("Get all users:\n");

	while ((pwd = getpwent()) != NULL)
		printf("\t%s: %ld\n", pwd->pw_name, (long)pwd->pw_uid);

	endpwent();

	struct group *grp;

	grp = getgrnam("wheel");
	if (!grp) {
		printf("Group not found! Aborting\n");
		return EXIT_FAILURE;
	}

	printf("Group wheel(using getgrnam):\n");
	printf("\tGroup name: %s\n", grp->gr_name);
	printf("\tGroup password: %s\n", grp->gr_passwd);
	printf("\tGroup GID: %d\n", grp->gr_gid);

	char **nam;
	for (nam = grp->gr_mem; *nam; nam++)
		printf("\tGroup member: %s\n", *nam);

	grp = getgrgid(grp->gr_gid);
	if (!grp) {
		printf("Group not foundby gid! Aborting\n");
		return EXIT_FAILURE;
	}

	printf("Group encontered by id %d\n", grp->gr_gid);

	printf("All groups:\n");
	while ((grp = getgrent()) != NULL)
		printf("\t%s\n", grp->gr_name);

	endgrent();

	return EXIT_SUCCESS;
}
