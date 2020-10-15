#include <stdio.h>
#include "ctx.h"
#include "rw.h"

#define MY_SIZE (1024 * 32)

void func1 (void *arg1)
{
	puts("func1 start");
	printf("arg1:%s\n", (char *)arg1);
	coroutine_yield();
	puts("func1 end");
}

void func2 (void *arg2)
{
	puts("func2 start");
	printf("arg2:%s\n", (char *)arg2);
	coroutine_yield();
	puts("func2 end");
}

int main (void)
{
	int read_id, write_id;
	char buf[512];
	read_id = coroutine_create(coroutine_read, buf, MY_SIZE);
	write_id = coroutine_create(coroutine_write, buf, MY_SIZE);
	coroutine_resume(read_id);
	coroutine_resume(write_id);
#if 0
	int co_id1, co_id2;
	char arg1[] = "hello";
	char arg2[] = "test";

	co_id1 = coroutine_create(func1, arg1, MY_SIZE);
	co_id2 = coroutine_create(func2, arg2, MY_SIZE);	
	coroutine_resume(co_id1);
	coroutine_resume(co_id2);
#endif
	puts("main exit");
	
	return 0;
}
