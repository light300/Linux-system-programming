#ifndef __CTX_H__
#define __CTX_H__

int coroutine_create (void (*func) (void *), void *args, unsigned int size);
int coroutine_yield (void);
int coroutine_resume (int co_id);

#endif
