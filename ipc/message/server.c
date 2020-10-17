#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define TO_SERVER_MSGTYPE 1000
#define CLIENT_NUMBER 3

struct msgbuf
{
	long mtype;
	int client_id;
	char mtext[80];
};

int main (void)
{
	int i;
	key_t key = ftok("./", 512);
	int msg_id = msgget (key, IPC_CREAT | 0666);
	struct msgbuf msg;

	while (1) {
		memset (&msg, 0, sizeof(msg));
		if (msgrcv(msg_id, (void *) &msg, sizeof(msg.mtext), \
				   TO_SERVER_MSGTYPE, 0) == -1) {
			perror ("msgrcv");
			exit (EXIT_FAILURE);
		} else {
			for(i = 1; i <= CLIENT_NUMBER; i++)
			{
				if (i == msg.client_id)
					continue;
				else
				{
					msg.mtype = i;
					msgsnd (msg_id, &msg, 80, 0);
				}
			}
		}
		printf ("server: %s\n", msg.mtext);
	}
	msgctl (msg_id, IPC_RMID, NULL);
	return 0;
}
