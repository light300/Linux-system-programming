#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

void signal_handler (int signo)
{
    printf("Catch signal: %d\n", signo);
}

int main (void)
{
    struct itimerval tv, tv_prev;
    signal (SIGALRM, signal_handler);
    
    tv.it_value.tv_sec = 5;
    tv.it_value.tv_usec = 0;
    tv.it_interval.tv_sec = 1;
    tv.it_interval.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &tv, &tv_prev) != 0)
    {
        printf("setitimer error %d\n", errno);
        return -1;
    }
    while(1);
    return 0;
}
