#include <stdio.h>
#include <time.h>

int main (void)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    printf("%ld:%ld\n", t.tv_sec, t.tv_nsec);
    
    t.tv_sec += 100;
    clock_settime(CLOCK_REALTIME, &t);

    struct timespec t2;
    clock_gettime(CLOCK_REALTIME, &t2);
    printf("%ld:%ld\n", t2.tv_sec, t2.tv_nsec);
    
    return 0;
}

