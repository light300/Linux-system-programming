#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

static void signal_handler(int signo)
{
    printf("catch signal %d\n", signo);
}
     
int main(void)
{
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;

	signal(SIGRTMIN, signal_handler);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
    	perror("timer_create");
    	exit(EXIT_FAILURE);
    }

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
