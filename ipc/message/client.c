#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define handle_error(msg) {perror(msg); \
                           exit(EXIT_FAILURE);}
#define TO_SERVER_MSGTYPE 1000

struct msgbuf
{
	long mtype;
	int client_id;
	char mtext[80];
};

int main (void)
{	
	int ret_from_fork;
	key_t key = ftok("./", 512);
	int msg_id = msgget(key, IPC_CREAT | 0666);
	struct msgbuf msg_snd, msg_rcv;

	printf ("input guest ID:");
	int client_id;
	scanf ("%d", &client_id);
	printf ("client_id: %d\n", client_id);

	ret_from_fork = fork();
	if (ret_from_fork == -1)
        handle_error("fork")	
	else if (ret_from_fork == 0) {
		while (1) {
			gets(msg_snd.mtext);
			msg_snd.mtype = TO_SERVER_MSGTYPE;
			msg_snd.client_id = client_id;
			msgsnd (msg_id, &msg_snd, 80, 0);
		}
	} else {
		while (1) {
			memset (&msg_rcv, 0, sizeof(msg_rcv));
			if (msgrcv (msg_id, (void *)&msg_rcv, \
                        sizeof(msg_rcv.mtext), client_id, 0) == -1)
			    handle_error("msg_rcv")
			else
				printf ("client%d: %s\n", msg_rcv.client_id, msg_rcv.mtext);
		}
	}

	return 0;
}
