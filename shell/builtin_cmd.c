#include <stdio.h>
#include <unistd.h>
#include <string.h>

int builtin_cmd_cd (char path[])
{
	int ret;
	ret = chdir(path);
	if (ret == -1) {
		printf("cd %s failed\n", path);
		return -1;
	}
	return 0;
}

int is_builtin_cmd (char *arglist[])
{
	if (strcmp(arglist[0], "cd") == 0) {
        return 1;
    }
	return 0;
}
