#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int read_cmd (char buffer[]);
int pars_cmd (char buffer[], char *arglist[]);
int exec_cmd (char *arglist[]);
int free_cmd (char *arglist[], int arg_num);

int main (void)
{
	char buffer[512] = {0};
	char *arglist[20] = {0};
	int arg_num = 0;

	while (1) {
		read_cmd (buffer);
		arg_num = pars_cmd (buffer, arglist);
		exec_cmd (arglist);
		free_cmd (arglist, arg_num);
		memset (buffer, '\0', 512);
		memset (arglist, 0, 80);
	}

	printf("main shell exit\n");

	return 0;
}

int read_cmd (char buffer[])
{
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
    return pos;
}

int pars_cmd (char buffer[], char *arglist[])
{
	char *cp = buffer;
    int arg_num = 0;
    int len;
    char *start;
    char c;
    while (*cp != '\0') {
        while (*cp == ' ' || *cp == '\t')
            cp++;
        if (*cp == 0)
			break;
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
	return arg_num;
}

int exec_cmd (char *arglist[])
{
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
	return 0;
}

int free_cmd (char *arglist[], int arg_num)
{
	int i;
    for (i = 0; i < arg_num; i++)
        free(arglist[i]);	
	return 0;
}
