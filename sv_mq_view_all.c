#define _GNU_SOURCE // for MSG_STAT, MSG_INFO

#include <sys/msg.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "error_aux.h"

int main()
{
	int maxind, ind, msqid;
	struct msqid_ds ds;
	struct msginfo msginfo;

	// size of kernel entries array
	maxind = msgctl(0, MSG_INFO, (struct msqid_ds *) &msginfo);
	if (maxind == -1)
		exit_failure("msgctl");

	printf("maxind: %d\n", maxind);
	printf("index	id	key	messages\n");

	for (ind = 0; ind <= maxind; ind++) {
		msqid = msgctl(ind, MSG_STAT, &ds);
		if (msqid == -1) {
			if (errno != EINVAL && errno != EACCES)
				exit_failure("msgstat"); // unkown error
			continue; //skip this one
		}
		
		printf("%4d	%8d	0x%08lx	%7ld\n", ind, msqid, (unsigned long)ds.msg_perm.__key, (long)ds.msg_qnum);
	}

	exit(EXIT_SUCCESS);
}
