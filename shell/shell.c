#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (void)
{
	char *arglist[5] = {"ls", "-l", "-a"};

	int ret_from_fork;
	int child_ret_status;
	ret_from_fork = fork();
	
	if (ret_from_fork == 0) {
		execvp(arglist[0], arglist);
		perror("execvp");
		exit(EXIT_FAILURE);
	} else if (ret_from_fork > 0) {
		if (wait(&child_ret_status) ==  -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
	} else {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	printf("main shell exit\n");

	return 0;
}
