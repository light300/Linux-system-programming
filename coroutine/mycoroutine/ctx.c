#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

#define STACK_SIZE (1024 * 64)

struct coroutine
{
	void (*func) (void *);
	void *args;
	char *stack;
	ucontext_t ctx;
};

ucontext_t ctx_main;
struct coroutine coroutine_slots[12];
int id_num = 0; // how many coroutine total numbers;
int current_id = 0; //save current id;

void coroutine_start (void)
{
	struct coroutine *t = &coroutine_slots[current_id];
	t->func(t->args);
}

int coroutine_create (void (*func) (void *), void *args, unsigned int size)
{
	struct coroutine *rt = &coroutine_slots[id_num];

	current_id = id_num;
	
	getcontext(&(rt->ctx));

	rt->func = func;
	rt->args = args;

/***set stack size
	if size equal zero use STACK_SIZE as default***/
	int real_size;
	if (size == 0)
		real_size = STACK_SIZE;
	else
		real_size = size;
	rt->stack = (char *) malloc (real_size);
    rt->ctx.uc_stack.ss_sp    = rt->stack;
    rt->ctx.uc_stack.ss_size  = real_size; 
    rt->ctx.uc_stack.ss_flags = 0;
    rt->ctx.uc_link           = &ctx_main;
    makecontext(&(rt->ctx), coroutine_start, 0);
	swapcontext(&ctx_main, &(rt->ctx));

	return id_num++;
}

int coroutine_yield (void)
{
	struct coroutine *t = &coroutine_slots[current_id];
	swapcontext(&(t->ctx), &ctx_main);
}

int coroutine_resume (int co_id)
{
    struct coroutine *t = &coroutine_slots[co_id];
	current_id = co_id;
    swapcontext(&ctx_main, &(t->ctx));
	free(t->stack);
	t->stack = NULL;
}
