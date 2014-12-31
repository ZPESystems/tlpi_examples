#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SHM_KEY 0x1234 // key for shared memory
#define SEM_KEY 0x5678 // key for semaphore set

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define WRITE_SEM 0
#define READ_SEM 1

#ifndef BUF_SIZE // can be override with "cc -DBUF_SIZE"
#define BUF_SIZE 1024
#endif

enum SemType {
	SEM_AVAILABLE,
	SEM_IN_USE,
	SEM_RESERVE,
	SEM_RELEASE
};

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short * array;
#if defined(__linux__)
	struct seminfo * __buf;
#endif
};

int setSem(enum SemType st, int semId, int semNum)
{
	if (st == SEM_AVAILABLE || st == SEM_IN_USE) {
		union semun arg;
		if (st == SEM_AVAILABLE)
			arg.val = 1;
		else
			arg.val = 0;
		return semctl(semId, semNum, SETVAL, arg);
	} else {
		struct sembuf sops = { .sem_num = semNum, .sem_flg = 0, .sem_op = (st == SEM_RESERVE) ? -1 : 1};
		return semop(semId, &sops, 1);
	}
}

struct shmseg { // struct to share in shmem
	int cnt; // number of bytes in 'buf'
	char buf[BUF_SIZE]; // data being transfered
};
