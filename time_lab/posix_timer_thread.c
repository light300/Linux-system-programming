#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>

void timer_thread(union sigval val)
{
    printf("catch args:%d\n", val.sival_int);
}
     
int main(void)
{
    timer_t timerid;
    struct sigevent sev;
    memset(&sev, 0, sizeof(struct sigevent));
    
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = timer_thread;
    sev.sigev_value.sival_int = 3;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
    	perror("timer_create");
    	exit(EXIT_FAILURE);
    }
    
    struct itimerspec its;
    its.it_value.tv_sec = 5;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
    	perror("timer_settime");
    	exit(EXIT_FAILURE);
    }

	while(1);
    timer_delete(timerid);
    exit(EXIT_SUCCESS);
}
