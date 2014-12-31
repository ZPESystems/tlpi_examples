#include "svshm.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int semid, shmid;
	struct shmseg *shmp;
	union semun dummy;

	if ((semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS)) == -1) {
		perror("semget");
		_exit(EXIT_FAILURE);
	}

	if (setSem(SEM_AVAILABLE, semid, WRITE_SEM) == -1) {
		perror("setSe, ava");
		_exit(EXIT_FAILURE);
	}
	if (setSem(SEM_IN_USE, semid, READ_SEM) == -1) {
		perror("setSe, in_use");
		_exit(EXIT_FAILURE);
	}

	if ((shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS)) == -1) {
		perror("shmget");
		_exit(EXIT_FAILURE);
	}

	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *)-1) {
		perror("shmat");
		_exit(EXIT_FAILURE);
	}

	// transfer data from stdin to shared memory
	int xfrs, bytes;
	for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
		if (setSem(SEM_RESERVE, semid, WRITE_SEM) == -1) { // wait for our turn
			perror("setSem reserve");
			_exit(EXIT_FAILURE);
		}

		if ((shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE)) == -1) {
			perror("read");
			_exit(EXIT_FAILURE);
		}

		if (setSem(SEM_RELEASE, semid, READ_SEM) == -1) { // give reader a turn
			perror("setSem release");
			_exit(EXIT_FAILURE);
		}

		// test EOF
		if (shmp->cnt == 0)
			break;
	}

	// wait for read finishs and delete IPC objects
	if (setSem(SEM_RESERVE, semid, WRITE_SEM) == -1) { // wait for our turn
		perror("setSem reserve2");
		_exit(EXIT_FAILURE);
	}

	if (semctl(semid, 0, IPC_RMID, dummy) == -1) {
		perror("setctl");
		_exit(EXIT_FAILURE);
	}

	if (shmdt(shmp) == -1) {
		perror("shmdt");
		_exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("shmctl");
		_exit(EXIT_FAILURE);
	}

	fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
	return EXIT_SUCCESS;
}
