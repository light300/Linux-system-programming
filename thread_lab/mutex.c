#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int cake_count = 0;
pthread_mutex_t mutex;

static void *produce_thread (void *arg)
{
	cake_count = *(int *) arg;
	while (1) {
		pthread_mutex_lock(&mutex);
		cake_count += 1;
		printf ("produce thread: cake_count = %d\n", cake_count);
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}
	printf ("sub thread1: exit\n");
	pthread_exit(NULL);
}

static void *consume_thread (void *arg)
{
	int consum_unit = *(int *) arg;
	while (1) {
		pthread_mutex_lock(&mutex);
		if (cake_count == 0) {
			pthread_mutex_unlock(&mutex);
			continue;
		}
		cake_count -= consum_unit;
		printf ("consume thread consume 1: cake_count = %d\n", cake_count);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	printf("sub thread2; exit\n");
	pthread_exit(NULL);
}

int main (void)
{
	pthread_t tid1, tid2;

	pthread_mutex_init(&mutex, NULL);

	int arg1 = 10;
	int arg2 = 1;
	pthread_create(&tid1, NULL, produce_thread, (void *) &arg1);
	pthread_create(&tid2, NULL, consume_thread, (void *) &arg2);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_mutex_destroy(&mutex);
	printf ("main thread: exit\n");
	
	return 0;
}
