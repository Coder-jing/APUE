#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

int main()
{
    int pd[2];
    pid_t pid;
    char* buf[1024];

    int err=pipe(pd);
    if(err<0)
    {
        perror("pipe()");
        exit(1);
    }

    pid=fork();
    if(pid<0)
    {
        perror("fork()");
        exit(1);
    }

    if(pid==0)
    {
        close(pd[1]);
        int len=read(pd[0],buf,1024);
        write(1,buf,len);
        exit(0);
    }
    else
    {
        close(pd[0]);
        write(pd[1],"hello world",11);
        close(pd[1]);
        wait(NULL);
        exit(0);
    }

    exit(0);
}