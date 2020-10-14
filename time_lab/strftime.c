#include <stdio.h>
#include <time.h>

int main (void)
{
    time_t t = time(NULL);
    printf("%ld\n", t);
    char *str;
    str = ctime(&t);
    printf("%s\n", str);
    
    struct tm m;
    strptime(str, "%a %b %d %H:%M:%S %Y", &m);
    printf("%d-%d-%d ", m.tm_year + 1900, m.tm_mon + 1, m.tm_mday);
    printf("%d:%d:%d\n", m.tm_hour, m.tm_min, m.tm_sec);

    char buf[512] = {0};
    strftime(buf, 512, "Now is: %Y %B %d %H:%M:%S", &m);
    printf("%s\n", buf);

    return 0;
}

