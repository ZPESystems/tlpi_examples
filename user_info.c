#define _DEFAULT_SOURCE  //getpass() from unistd.h
#define _XOPEN_SOURCE // crypto from unistd.h

#include <pwd.h>
#include <grp.h>
#include <shadow.h>

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void validate_user(struct passwd *pwd, struct spwd *s)
{
	char *passwd, *encrypted;

	passwd = getpass("Password: ");

	// use shadow passwd if exists
	if (s) {
		printf("Shadow passwd: %s\n", s->sp_pwdp);
		pwd->pw_passwd = s->sp_pwdp;
	}

	encrypted = crypt(passwd, pwd->pw_passwd);
	// erase cleartext password
	bzero(&passwd, sizeof(passwd));

	if (!encrypted) {
		perror("crypt");
		return;
	}

	if (strcmp(encrypted, pwd->pw_passwd)) {
		printf("Wrong password...\n");
		return;
	}

	printf("enc passwd: %s\n", encrypted);
	printf("enc passwd stored: %s\n", pwd->pw_passwd);

	printf("User %s authenticated! UID=%d\n", pwd->pw_name, pwd->pw_uid);
}

int main()
{
	struct passwd *pwd, *pw;
	pwd = getpwnam("marcos");

	if (!pwd) {
		printf("User not found! Aborting\n");
		return EXIT_FAILURE;
	}

	printf("User marcos:\n");
	printf("\tHome dir: %s\n", pwd->pw_dir);
	printf("\tLogin shell: %s\n", pwd->pw_shell);
	printf("\tGroup id: %d\n", pwd->pw_gid);
	printf("\tPassword: %s\n", pwd->pw_passwd);

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

	while ((pw = getpwent()) != NULL)
		printf("\t%s: %ld\n", pw->pw_name, (long)pw->pw_uid);

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

	struct spwd *s;
	s = getspnam("marcos");

	if (s) {
		printf("Shadow data:\n");
		printf("\tUsername: %s\n", s->sp_namp);
		printf("\tEncrypted passwd: %s\n", s->sp_pwdp);
		printf("\tLast passwd changed: %ld\n", s->sp_lstchg);

		validate_user(pwd, s);
	} else {
		perror("getspnam");
	}

	return EXIT_SUCCESS;
}
