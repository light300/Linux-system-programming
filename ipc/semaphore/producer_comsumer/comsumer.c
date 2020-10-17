#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

int sem_id;

int mem_read (int *addr)
{
	int index;
	int data;
	index = addr[0];
	data  = addr[index];
	index--;
	addr[0] = index;
	return data;
}

void sem_init (int semid, int nsignum, int sem_value)
{
	union semun sem_union;
	sem_union.val = sem_value;
	if (semctl(semid, nsignum, SETVAL, sem_union) == -1) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}
}

void sem_p (int semid, int nsignum)
{
	struct sembuf sops;
	sops.sem_num = nsignum;
	sops.sem_op  = -1;
	sops.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sops, 1) == -1) {
		perror("semop");
		exit(EXIT_FAILURE);
	}
}

void sem_v (int semid, int nsignum)
{
	struct sembuf sops;
	sops.sem_num = nsignum;
	sops.sem_op  = 1;
	sops.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sops, 1) == -1) {
		perror("semop");
		exit(EXIT_FAILURE);
	}
}

void sem_print (int sem_id, int nsignum)
{
	int sem_value;
	sem_value = semctl(sem_id, nsignum, GETVAL);
	printf ("sem[%d] = %d\n", nsignum, sem_value);
}

int main (void)
{
	int shm_id;
	key_t shm_key = ftok("./", 5161);
	key_t sem_key = ftok("./", 5162);

	shm_id = shmget(shm_key, 1028, IPC_CREAT | 0644);
	char *shm_addr = shmat(shm_id, NULL, 0);
	memset(shm_addr, 0, 128);

	sem_id = semget(sem_key, 2, IPC_CREAT | 0644);
	if (sem_id == -1 ) {
		perror ("semget");
		exit (EXIT_FAILURE);
	} else {
		sem_init(sem_id, 0, 0); // init read semaphore
		sem_init(sem_id, 1, 5); // init  write semaphore
	}

	for (int i = 0; i < 2; i++) {
		int ret_from_fork;
		if ((ret_from_fork = fork()) == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		} else if (ret_from_fork == 0) {
			while (1) {
				sleep(2);
				sem_p(sem_id, 0);
				printf("pid %d data: %d\n", getpid(), mem_read((int *) shm_addr));
				sem_v(sem_id, 1);
			}
		}
	}

    while (1) {           //read
		sleep(5);
		sem_p(sem_id, 0);
		printf("pid %d data: %d\n", getpid(), mem_read((int *) shm_addr));
		sem_v(sem_id, 1);
	}
	return 0;
}
