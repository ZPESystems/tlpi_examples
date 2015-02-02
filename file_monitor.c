#include <sys/inotify.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "error_aux.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void display_event(struct inotify_event *e)
{
	printf("watch descriptor = %2d; ", e->wd);
	if (e->cookie > 0)
		printf("cookie =%4d; ", e->cookie);

	printf("mask = ");
	if (e->mask & IN_ACCESS) 	printf("IN_ACCESS ");
	if (e->mask & IN_ATTRIB) 	printf("IN_ATTRIB ");
	if (e->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
	if (e->mask & IN_CLOSE_WRITE) 	printf("IN_CLOSE_WRITE ");
	if (e->mask & IN_CREATE) 	printf("IN_CREATE ");
	if (e->mask & IN_DELETE) 	printf("IN_DELETE ");
	if (e->mask & IN_DELETE_SELF) 	printf("IN_DELETE_SELF ");
	if (e->mask & IN_IGNORED) 	printf("IN_IGNORED ");
	if (e->mask & IN_ISDIR) 	printf("IN_ISDIR ");
	if (e->mask & IN_MODIFY) 	printf("IN_MODIFY ");
	if (e->mask & IN_MOVE_SELF) 	printf("IN_MOVE_SELF ");
	if (e->mask & IN_MOVED_FROM) 	printf("IN_MOVED_FROM ");
	if (e->mask & IN_MOVED_TO) 	printf("IN_MOVED_TO ");
	if (e->mask & IN_OPEN) 		printf("IN_OPEN ");
	if (e->mask & IN_Q_OVERFLOW) 	printf("IN_Q_OVERFLOW ");
	if (e->mask & IN_UNMOUNT) 	printf("IN_UNMOUNT ");

	printf("\n");

	if (e->len > 0)
		printf("	name = %s\n", e->name);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Inform your path in argv. Aborting\n");
		return EXIT_FAILURE;
	}

	int ifd = inotify_init();
	if (ifd == -1)
		exit_failure("inotify_init");

	int i;
	for (i = 1; i < argc; i++) {
		int err = inotify_add_watch(ifd, argv[i], IN_ALL_EVENTS);
		if (err == -1)
			exit_failure("inotify_add_watch");

		printf("Watching %s file using %d fd\n", argv[i], err);
	}

	char buf[BUF_LEN];
	char *p;
	struct inotify_event *event;

	for (;;) {
		int numRead = read(ifd, buf, BUF_LEN);
		if (numRead <= 0)
			exit_failure("read");

		printf("Read %ld bytes from inotify fd\n", (long)numRead);

		for (p = buf; p < buf + numRead; ) {
			event = (struct inotify_event *)p;
			display_event(event);

			p += sizeof(struct inotify_event) + event->len;
		}
	}

	return EXIT_SUCCESS;
}
