#include <stdio.h>
#include <unistd.h>

int main (void)
{
	char *arglist[5] = {"ls", "-l", "-a"};

	execvp(arglist[0], arglist);
	printf("main shell exit\n");

	return 0;
}
