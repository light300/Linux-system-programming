#include <stdio.h>
#include <time.h>

int main (void)
{
	time_t t = time(NULL);
	printf("%ld\n", t);

	struct tm *p;
	p = localtime(&t);
	printf("%d-%d-%d ",p->tm_year + 1900, p->tm_mon + 1, p->tm_mday);
	printf("%d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);

	p = gmtime(&t);
	printf("%d-%d-%d ",p->tm_year + 1900, p->tm_mon + 1, p->tm_mday);
	printf("%d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
	
	return 0;
}
