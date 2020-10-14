#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void SIGALRM_handler (int signo)
{
    printf ("Got signal: %d\n", signo);
}

int main (void)
{
   signal(SIGALRM, SIGALRM_handler);
    
    alarm(3);
    sleep(5);
    puts("main exit");
    return 0;
}


