#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define BUFSIZE 1024

int main()
{
    time_t stamp;
    struct tm* s_tm;
    char buf[BUFSIZE];

    stamp=time(NULL);
    s_tm=localtime(&stamp);

    strftime(buf,BUFSIZE,"Now:%Y-%m-%d\n",s_tm);
    puts(buf);

    s_tm->tm_mday+=100;
    mktime(s_tm);
    strftime(buf,BUFSIZE,"+100day:%Y-%m-%d\n",s_tm);
    puts(buf);

    exit(0);
}
