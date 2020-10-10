#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct ctl_cmd_info {
    int if_result;
    int if_block;
    int else_block;
};

int is_control_cmdline (char buffer[])
{
    return (strncmp(buffer, "if", 2) == 0   || \
            strncmp(buffer, "else", 4) == 0 || \
            strncmp(buffer, "fi", 2) == 0);
}

int is_exec_cmdline (struct ctl_cmd_info cmd_info)
{
    if ((cmd_info.if_result && cmd_info.if_block)          ||  \
        ((cmd_info.if_result == 0) && cmd_info.else_block) || \
        ((cmd_info.if_block == 0) && (cmd_info.else_block == 0)))
    	return 1;
    else
        return 0;
}

static int is_if_cmdline_true (char buffer[])
{
    char *arg_array[10] = {0};
    char *cp = buffer;
    int len = 0;
    char c;
    int args_num = 0;
    char *start;
    while (*cp != '[')
        cp++;
    cp++;
    while (strncmp(cp, "]", 1) != 0) {
        while (*cp == ' ')
            cp++;
        if (strncmp(cp, "]", 1) == 0)
            break;
        len = 1;
        start = cp;
        c = *++cp;
        while (c != ' ' && c != '\t') {
            len++;
            c = *++cp;
        }
        arg_array[args_num] = (char *) malloc (len);
        strncpy(arg_array[args_num++], start, len);
		arg_array[args_num-1][len] = '\0';
    }
    if (strncmp(arg_array[0], "-f", 2) == 0) {
        int result = 0;
        if ((access(arg_array[1], F_OK)) != -1)
            result = 1;
        free(arg_array[0]);
        free(arg_array[1]);
        return result;
    }
    if (strncmp (arg_array[0], "-d", 2) == 0) {
        int result = 0;
        if ((access (arg_array[1], F_OK)) != -1)
            result = 1;
        free (arg_array[0]);
        free (arg_array[1]);
        return result;
    }
    return 0;
}
 
void process_control_cmdline (char buf[], struct ctl_cmd_info *cmd_info)
{
    if (strncmp(buf, "if", 2) == 0) {
        cmd_info->if_block = 1;
        cmd_info->else_block = 0;
        if (is_if_cmdline_true(buf))
            cmd_info->if_result = 1;
    }
    if (strncmp (buf, "else", 4) == 0) {
        cmd_info->if_block = 0;
        cmd_info->else_block = 1;
    }
    if (strncmp (buf, "fi", 2) == 0) {
        cmd_info->if_block = 0;
        cmd_info->else_block = 0;
        cmd_info->if_result = 0;
    }
}
