#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <time.h>
#include <signal.h>

int read_cmd (char buffer[]);
int pars_cmd (char buffer[], char *arglist[]);
int exec_cmd (char *arglist[], int fd_in, int fd_out);
int exec_cmdline  (char *arglist[], char buffer[]);
int free_cmd (char *arglist[], int arg_num);
void print_cmd_user(void);

int background_exec = 0;
/*
	1: background exec mode
	0: normal mode
*/

int pipe_mode = 0;
/*
    1: pipe mode
    0: normal mode
*/

int main (void)
{
	char buffer[512] = {0};
	char *arglist[20] = {0};

	while (1) {
		print_cmd_user();
		read_cmd (buffer);
		exec_cmdline (arglist, buffer);
		memset (buffer, '\0', 512);
		memset (arglist, 0, 80);
	}

	printf("main shell exit\n");

	return 0;
}

void print_cmd_user(void)
{
	struct passwd *pwd;
	pwd = getpwuid(getuid());
	char path[256];
	getcwd(path, 256);
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	printf("\033[35;1;40m%s@mysh:\033[34m%s\033[33m [%d:%d:%d] \033[32m\n>>> \033[0m", pwd->pw_name, path, p->tm_hour, p->tm_min, p->tm_sec);
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
	while (*cp != '\0' && *cp != '\n') {
		if (*cp == '&') {
			background_exec = 1;
			*cp = '\0';
			break;
		}
		cp++;
	}
	cp = buffer;
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

int is_builtin_cmd (char *arglist[]);
int exec_builtin_cmd (char *arglist[]);

int exec_cmd (char *arglist[], int fd_in, int fd_out)
{
	int ret_from_fork;
    int child_ret_status = 0;
    
    ret_from_fork = fork();
    
    if (ret_from_fork == 0) {
		signal(SIGINT, SIG_DFL);

		dup2 (fd_in, 0);
		dup2 (fd_out, 1);
		if (fd_in != 0)
            close (fd_in);
        if (fd_out != 1)
            close (fd_out);

        execvp(arglist[0], arglist);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (ret_from_fork > 0) {
        signal(SIGINT, SIG_IGN);
		if (background_exec == 1) {
			background_exec = 0;
			signal(SIGCHLD, SIG_IGN);
		} else { 
			signal(SIGCHLD, SIG_DFL);
			if (wait(&child_ret_status) ==  -1) {
            	perror("wait");
            	exit(EXIT_FAILURE);
        	}
		}
		
		if (fd_in != 0)
            close (fd_in);
        if (fd_out != 1)
            close (fd_out);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
	return child_ret_status;
}

int is_pipe_command (char buffer[])
{
    char *cp = buffer;
    while (*cp != '\n' && *cp != '\0')
    {
        if (strncmp (cp, "|", 1) == 0)
            pipe_mode++;
			//return 1;
        cp++;
    }
    //return 0;
	return pipe_mode;
}

int exec_cmdline (char *arglist[], char buffer[])
{
	char *temp = buffer;
	char *args;
	int args_num = 0;
	int fd_tmp = 0;

	pipe_mode = is_pipe_command(buffer);

	while ((args = strtok(temp, "|")) != NULL) {
		temp = NULL;
		args_num = pars_cmd(args, arglist);
		if (arglist[0] == 0)
			return 0;
		if (is_builtin_cmd(arglist)) {
        	exec_builtin_cmd(arglist);
			return 0;
		}
		//if (pipe_mode == 1) {
		if (pipe_mode > 0) {
			int fd[2];
			if (pipe(fd) == -1) {
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			exec_cmd(arglist, fd_tmp, fd[1]);
			fd_tmp = fd[0];
			//pipe_mode = 0;
			pipe_mode--;
		} else {
			exec_cmd(arglist, fd_tmp, 1);
		}
		free_cmd(arglist, args_num);
        memset(arglist, 0, 80);
	}
	fd_tmp = 0;
	pipe_mode = 0;
	return 0;
}

int free_cmd (char *arglist[], int arg_num)
{
	int i;
    for (i = 0; i < arg_num; i++)
        free(arglist[i]);	
	return 0;
}
