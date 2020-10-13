#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int current_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

static void *produce_thread (void *arg)
{
	int count_init = *(int *) arg;
	current_count = count_init;
	while (1) {
		pthread_mutex_lock(&mutex);
		current_count++;
		printf ("produce thread: current_count = %d\n", current_count);
		if (current_count % 10 == 0)
			pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	printf ("sub thread1: exit\n");
	pthread_exit(NULL);
}

static void *consume_thread (void *arg)
{
	int consume_unit = *(int *) arg;
	while (1) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		current_count -= 10;
		printf ("consume thread has consume %d\n", consume_unit);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	printf("consume thread: exit\n");
	pthread_exit(NULL);
}

int main (void)
{
	pthread_t tid1, tid2;

	pthread_mutex_init(&mutex, NULL);

	int arg1 = 5;
	int arg2 = 10;
	pthread_create(&tid1, NULL, produce_thread, (void *) &arg1);
	pthread_create(&tid2, NULL, consume_thread, (void *) &arg2);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	printf ("main thread: exit\n");
	
	return 0;
}
