#include <stdio.h>
#include "ctx.h"

void coroutine_read (void *args)
{
	puts("coroutine_read start");
	coroutine_yield();
	int data = *(char *) args;
	printf("read coroutine read data: %d\n", data);
	puts("coroutine_read exit");
}

void coroutine_write (void *args)
{
	puts("coroutine_write start");
	*(char *) args = 100;
	puts("wirte coroutine write data success");
	coroutine_yield();
	puts("coroutine_write exit");
}
