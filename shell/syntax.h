#ifndef __SYNTAX_H
#define __SYNTAX_H

struct ctl_cmd_info{
    int if_result;
    int if_block;
    int else_block;
};
int is_control_cmdline (char buffer[]);
int is_exec_cmdline (struct ctl_cmd_info cmd_info);
void process_control_cmdline (char buf[], struct ctl_cmd_info *cmd_info);

#endif
