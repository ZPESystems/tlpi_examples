#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "svshm.h"

int main()
{
	int semid, shmid;
	struct shmseg *shmp;

	if ((semid = semget(SEM_KEY, 0, 0)) == -1) {
		perror("semget");
		_exit(EXIT_FAILURE);
	}

	if ((shmid = shmget(SHM_KEY, 0, 0)) == -1) {
		perror("shmget");
		_exit(EXIT_FAILURE);
	}

	if ((shmp = shmat(shmid, NULL,SHM_RDONLY)) == (void *)-1) {
		perror("shmat");
		_exit(EXIT_FAILURE);
	}

	// transfer data from shm to stdout
	int xfrs, bytes;
	for (xfrs = 0, bytes = 0; ; xfrs++) {
		if (setSem(SEM_RESERVE, semid, READ_SEM) == -1) { // wait for our turn
			perror("setSem reserve");
			_exit(EXIT_FAILURE);
		}

		if (shmp->cnt == 0) // EOF
			break;

		bytes += shmp->cnt;
		if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt) {
			perror("write");
			_exit(EXIT_FAILURE);
		}

		if (setSem(SEM_RELEASE, semid, WRITE_SEM) == -1) { // give writer a turn
			perror("setsem release");
			_exit(EXIT_FAILURE);
		}

	}

	if (shmdt(shmp) == -1) {
		perror("shmdt");
		_exit(EXIT_FAILURE);
	}

	// give writer one more turn, so it can clean up
	if (setSem(SEM_RELEASE, semid, WRITE_SEM) == -1) { // give writer a turn
		perror("setsem release");
		_exit(EXIT_FAILURE);
	}

	fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
	return EXIT_SUCCESS;
}
