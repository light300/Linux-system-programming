#include <stdio.h>
#include <time.h>

int main (void)
{
	time_t t = time(NULL);
	printf("%ld\n", t);
	t = t - 100;
	//stime(&t);
	//t = time(NULL);
	//printf("%ld\n", t);
	return 0;
}
