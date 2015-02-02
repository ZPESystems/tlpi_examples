#include "svshm.h"
#include "error_aux.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int semid, shmid;
	struct shmseg *shmp;
	union semun dummy;

	if ((semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS)) == -1)
		exit_failure("semget");

	if (setSem(SEM_AVAILABLE, semid, WRITE_SEM) == -1)
		exit_failure("setSem ava");

	if (setSem(SEM_IN_USE, semid, READ_SEM) == -1)
		exit_failure("setSem in_use");

	if ((shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS)) == -1)
		exit_failure("shmget");

	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *)-1)
		exit_failure("shmat");

	// transfer data from stdin to shared memory
	int xfrs, bytes;
	for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
		if (setSem(SEM_RESERVE, semid, WRITE_SEM) == -1) // wait for our turn
			exit_failure("setSem reserver");

		if ((shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE)) == -1)
			exit_failure("read");

		if (setSem(SEM_RELEASE, semid, READ_SEM) == -1) // give reader a turn
			exit_failure("setSem release");

		// test EOF
		if (shmp->cnt == 0)
			break;
	}

	// wait for read finishs and delete IPC objects
	if (setSem(SEM_RESERVE, semid, WRITE_SEM) == -1) // wait for our turn
		exit_failure("setSem reserve2");

	if (semctl(semid, 0, IPC_RMID, dummy) == -1)
		exit_failure("setctl");

	if (shmdt(shmp) == -1)
		exit_failure("shmdt");

	if (shmctl(shmid, IPC_RMID, 0) == -1)
		exit_failure("chmctl 2");

	fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
	return EXIT_SUCCESS;
}
