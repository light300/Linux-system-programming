#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main (void)
{
    time_t t;
    t = time(NULL);
    printf("%ld\n", t);

    struct timeval t1;
    gettimeofday(&t1, NULL);
    printf("%ld.%d seconds\n", t1.tv_sec, t1.tv_usec);
    
    t1.tv_sec += 100;
    settimeofday(&t1, NULL);
    
    struct timeval t2;
    gettimeofday(&t2, NULL);
    printf("%ld.%d seconds\n", t2.tv_sec, t2.tv_usec);
    return 0;
}

