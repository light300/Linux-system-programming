#include <stdio.h>
#include <string.h>

#define MAX_VAL 10

struct val {
	char name[20];
	char value[40];
};

int val_num = 0;
struct val val_array[MAX_VAL];

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
	return 0;
}

void env_cmd_set  (void)
{
	int i;
	for (i = 0; i< val_num; i++)
		printf("%s=%s\n", val_array[i].name, val_array[i].value);
}

int env_cmd_store (char *arglist[])
{
	char *temp = arglist[0];
	int i = 0;
	if (val_num >= MAX_VAL) {
		printf("val store space full!\n");
		return -1;
	}
	while (*temp != '=') {
		val_array[val_num].name[i++] = *temp;
		temp++;
	}
	val_array[val_num].name[i] = '\0';
	temp++;
	i = 0;
	while (*temp != '\0') {
		val_array[val_num].value[i++] = *temp;
		temp++;
	}
	val_array[val_num].value[i] = '\0';
	val_num++;
	return 0;
}

int exec_env_cmdline (char *arglist[])
{
	if (strcmp(arglist[0], "set") == 0)
		env_cmd_set();
	if (search_char('=', arglist))
		env_cmd_store(arglist);
	return 0;
}
