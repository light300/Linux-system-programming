#ifndef __ENV_H
#define __ENV_H

struct val_struct {
    char name[20];
	char value[40];
};

void shell_env_setup (void);
void shell_env_cleanup (void);
int is_env_command (char *arglist[]);                                       
int exec_env_cmdline (char *arglist[]);

#endif
