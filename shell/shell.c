#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (void)
{
	char buffer[512] = {0};
	char c;
	int pos = 0;
	while (1) {
		c = getchar();
		if (c != '\n' && c != EOF) {
			buffer[pos++] = c;
		} else {
			buffer[pos++] = '\0';
			break;
		}
	}

	char *arglist[20] = {0};
	char *cp = buffer;
	int arg_num = 0;
	int len;
	char *start;
	while (*cp != '\0') {
		while (*cp == ' ' || *cp == '\t')
			cp++;
		start = cp;
		len = 1;
		c = *++cp;
		while (c != '\0' && c != ' ' && c != '\t') {
			len++;
			c = *++cp;
		}
		arglist[arg_num] = (char *) malloc(len + 1);
		strncpy(arglist[arg_num], start, len);
		arglist[arg_num][len] = '\0';
		arg_num++;
	}
#if 0
	int i =0;
	while (arglist[i]) {
		printf("%s\n", arglist[i]);
		free(arglist[i]);
		i++;
	}
	return 0;
#endif
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
	
	int i =0;
    while (arglist[i])
 	    free(arglist[i++]);

	printf("main shell exit\n");

	return 0;
}
