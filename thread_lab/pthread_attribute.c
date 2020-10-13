#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int sum = 0;

void *thread_start (void *arg)
{
	int i;
	int len = *(int *) arg;
	for (i = 0; i < len; i++) {
		sum += i;
		printf("sub thread sum = %d\n", sum);
	}
	printf("sub thread exit\n");
	pthread_exit(NULL);
}

int main (void)
{
	int status;
	pthread_t tid;
	int arg = 10;

	pthread_attr_t attr;
	status = pthread_attr_init(&attr);
	if (status != 0) {
		perror("pthread_attr_init");
		exit(EXIT_FAILURE);
	}

	size_t stacksize = 0;
	pthread_attr_getstacksize(&attr, &stacksize);
	printf("stacksize = %lu kb\n", stacksize/1024);
	stacksize = 2 * 1024 *1024;
	pthread_attr_setstacksize(&attr, stacksize);
	stacksize = 0;
	pthread_attr_getstacksize(&attr, &stacksize);
	printf("stacksize = %lu kb\n", stacksize/1024);

	size_t detachstate;
	pthread_attr_getstacksize(&attr, &detachstate);
	printf("thread detach state: %zu\n", detachstate);
	pthread_attr_setstacksize(&attr, PTHREAD_CREATE_DETACHED);
	//pthread_attr_setstacksize(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_getstacksize(&attr, &detachstate);
	printf("thread detach state: %zu\n", detachstate);

	pthread_create(&tid, &attr, thread_start, (void *) &arg);
	pthread_attr_destroy(&attr);

	//pthread_join(tid, NULL);
	printf("main thread: sum = %d\n", sum);
	printf("main thread: exit\n");
	pthread_exit(NULL);

	return 0;
}
