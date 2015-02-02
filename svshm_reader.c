#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "svshm.h"
#include "error_aux.h"

int main()
{
	int semid, shmid;
	struct shmseg *shmp;

	if ((semid = semget(SEM_KEY, 0, 0)) == -1)
		exit_failure("semget");

	if ((shmid = shmget(SHM_KEY, 0, 0)) == -1)
		exit_failure("shmget");

	if ((shmp = shmat(shmid, NULL,SHM_RDONLY)) == (void *)-1)
		exit_failure("shmat");

	// transfer data from shm to stdout
	int xfrs, bytes;
	for (xfrs = 0, bytes = 0; ; xfrs++) {
		if (setSem(SEM_RESERVE, semid, READ_SEM) == -1) // wait for our turn
			exit_failure("setSem reserve");

		if (shmp->cnt == 0) // EOF
			break;

		bytes += shmp->cnt;
		if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt) {
			printf("write");
			exit(EXIT_FAILURE);
		}

		if (setSem(SEM_RELEASE, semid, WRITE_SEM) == -1) // give writer a turn
			exit_failure("setsem release");
	}

	if (shmdt(shmp) == -1)
		exit_failure("shmdt");

	// give writer one more turn, so it can clean up
	if (setSem(SEM_RELEASE, semid, WRITE_SEM) == -1) // give writer a turn
		exit_failure("setsem release");

	fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
	return EXIT_SUCCESS;
}
