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

int sem_id;

void mem_write (int *addr, int data)
{
	int index;
	index = addr[0];
	index++;
	addr[index] = data;
	addr[0] = index;
	//printf ("index = %c\n", index);
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
	printf("sem[%d] = %d\n", nsignum, sem_value);
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
	if (sem_id == -1)
		sem_id = semget (sem_key, 2, 0644);
	else {
		sem_init (sem_id, 0, 0); // for read
		sem_init (sem_id, 1, 5); // for write
	}
	
	int ret_from_fork;
	if ((ret_from_fork = fork()) == -1) {
		perror ("fork");
		exit (EXIT_FAILURE);
	} else if (ret_from_fork == 0) {
		int child_data = 1;
        while (1) {          //child process write
			sleep(1);
			sem_p(sem_id, 1);
			printf("child data: %d\n", child_data);
			mem_write((int *) shm_addr, child_data);
			child_data = child_data + 2;
			sem_v(sem_id, 0);
		}
	} else {
		int parent_data = 2;
        while (1) {          //parent process write
			sleep(1);
			sem_p(sem_id, 1);
			printf("parent data: %d\n", parent_data);
			mem_write((int *) shm_addr, parent_data);
			parent_data = parent_data + 2;
			sem_v (sem_id, 0);
		}
	}

	return 0;
}
