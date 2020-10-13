#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int sum = 0;

void *thread_start (void *arg)
{
	int i;
	int len = *(int *) arg;
	for (i = 0; i < len; i++) {
		sum += i;
		sleep(1);
		printf("sub thread sum = %d\n", sum);
	}
	printf("sub thread exit\n");
	pthread_exit(NULL);
}

int main (void)
{
	pthread_t tid;
	int arg = 10;
	pthread_create(&tid, NULL, thread_start, (void *) &arg);
	pthread_join(tid, NULL);
	printf("sum = %d\n", sum);
	printf("main thread: exit\n");
	return 0;
}
