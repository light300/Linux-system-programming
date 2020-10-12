#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAL 10

struct val_struct {
	char name[20];
	char value[40];
};

int env_count = 0;
char *env_array[256] = {0};

extern char **environ;

void shell_env_setup (void)
{
    while (environ[env_count])
    {
        int len = strlen (environ[env_count]);
        env_array[env_count] = (char *)malloc(len + 1);
        strncpy(env_array[env_count], environ[env_count], len);
        env_count++;
    }
}

void shell_env_cleanup (void)
{
    int i = 0;
    while (env_array[i])
        free(env_array[i++]);
}

int search_char (char ch, char *arglist[])
{
	char *p = arglist[0];
	while (*p != '\0') {
		if (ch == *p)
			return 1;
		p++;
	}
	return 0;
}

int is_env_command (char *arglist[])
{
	if (strcmp(arglist[0], "set") == 0)
		return 1;
	if (search_char('=', arglist))
		return 1;
	if(strcmp (arglist[0], "export") == 0)
		return 1;
	return 0;
}

int is_export_cmd (char *arglist[])
{
    if (strcmp(arglist[0], "export") == 0)
        return 1;
    return 0;
}

void env_cmd_set  (void)
{
	int i = 0;
	while (env_array[i])
		printf("%s\n", env_array[i++]);
}

int env_cmd_export (char *arglist[])
{
    struct val_struct val = {{0},{0}};  
    char *temp = arglist[1];
    int i = 0;
    while (*temp != '=')
    {
        val.name[i++] = *temp;
        temp++;
    }
    val.name[i] = '\0';
    temp++;
    i = 0;
    while (*temp != '\0')
    {
        val.value[i++] = *temp;
        temp++;
    }
    val.value[i] = '\0';
    setenv (val.name, val.value, 1);
    return 0;
}

int env_cmd_store (char *arglist[])
{
	char *temp;
	if (is_export_cmd(arglist))
		temp = arglist[1];
	else
		temp = arglist[0];
	int len = strlen(temp);
	env_array[env_count] = (char *)malloc(len);
	strcpy(env_array[env_count], temp);
	env_count++;
	return 0;
}

int exec_env_cmdline (char *arglist[])
{
	if (strcmp(arglist[0], "set") == 0)
		env_cmd_set();
	if (search_char('=', arglist))
		env_cmd_store(arglist);
	if (strcmp(arglist[0], "export") == 0)
    {
        env_cmd_store (arglist);
        env_cmd_export (arglist);
    }
	return 0;
}
