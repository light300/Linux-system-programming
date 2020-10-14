#include <stdio.h>
#include <time.h>

int main (void)
{
    time_t t = time(NULL);
    printf("%ld\n", t);
    char *str = NULL;
    str = ctime(&t);
    printf("%s\n", str);

    struct tm *p;
    p = gmtime(&t);
    printf("%d-%d-%d ", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday);
    printf("%d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
    
    str = asctime(p);
    printf("%s", str);

    return 0;
}

