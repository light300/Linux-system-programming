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

/*union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};*/

int sem_id;

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
	printf("sem[%d] = %d\n", nsignum, sem_value);
}

int main (void)
{
	int shm_id;
	key_t shm_key = ftok("./", 5151);
	key_t sem_key = ftok("./", 5152);

	shm_id = shmget(shm_key, 1028, IPC_CREAT | 0644);
	char *shm_addr = shmat(shm_id, NULL, 0);

	sem_id = semget(sem_key, 2, IPC_CREAT | 0644);
	if (sem_id == -1 ) {
		perror("semget");
		exit(EXIT_FAILURE);
	} else {
		sem_init(sem_id, 0, 0);//sem[0]: for read
		sem_init(sem_id, 1, 1);//sem[1]: for write
	}

	while (1) {            //read
		sem_p (sem_id, 0);
		printf ("from shm:%s", shm_addr);
		sem_v (sem_id, 1);
	}

	return 0;
}
