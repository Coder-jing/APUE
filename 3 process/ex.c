#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    puts("Begin!");
    fflush(NULL);
    execl("/usr/bin/date","date","+%s",NULL);
    perror("execl()");
    exit(1);


    puts("End!\n");
    exit(0);
}