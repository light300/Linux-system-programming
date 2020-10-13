#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int cake_count = 0;
pthread_rwlock_t rwlock;

void *write_thread (void *arg)
{
    static int id = 0;
    int i = ++id;
    int produce = *(int *) arg;
    while (1)
    {
        pthread_rwlock_wrlock(&rwlock);
        cake_count += produce;
        printf ("produce thread %d: cake_count = %d\n", i, cake_count);
        pthread_rwlock_unlock(&rwlock);
        sleep (1);
    }
    printf("produce thread: exit\n");
    pthread_exit(NULL);
}

void *read_thread (void *arg)
{
    static int id = 0;
    int i = ++id;
    int consume_unit = *(int *) arg;
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
#if 0
		if (cake_count <= 0) {
			pthread_rwlock_unlock(&rwlock);
			continue;
		}
		cake_count -= consume_unit;
#endif
		printf ("consume thread %d  cake_count = %d\n", i, cake_count);
        pthread_rwlock_unlock(&rwlock);
        sleep (1);
    }
    printf ("consume thread %d: exit\n", i);
    pthread_exit(NULL);
}

int main (void)
{
    pthread_t produce_tid1, produce_tid2;
    pthread_t consume_tid1, consume_tid2, consume_tid3;
    int produce1 = 10; // produce per second
    int produce2 = 20;
    int consume1 = 5;  // consume per second
    int consume2 = 10;
    int consume3 = 15;

    pthread_rwlock_init(&rwlock, NULL);
    pthread_create(&produce_tid1, NULL, write_thread, (void *) &produce1);
    pthread_create(&produce_tid2, NULL, write_thread, (void *) &produce2);
    pthread_create(&consume_tid1, NULL, read_thread, (void *) &consume1);
    pthread_create(&consume_tid2, NULL, read_thread, (void *) &consume2);
    pthread_create(&consume_tid3, NULL, read_thread, (void *) &consume3);
    pthread_join(produce_tid1, NULL);
    pthread_join(produce_tid2, NULL);
    pthread_join(consume_tid1, NULL);
    pthread_join(consume_tid2, NULL);
    pthread_join(consume_tid3, NULL);
    
    pthread_rwlock_destroy(&rwlock);
    printf ("main thread: exit\n");
    return 0;
}

