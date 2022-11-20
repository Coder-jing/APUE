#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include"relayer.h"


#define TTY1    "/dev/tty11"
#define TTY3    "/dev/tty33"
#define TTY4    "/dev/tty44"
#define TTY5    "/dev/tty55"

#define BUFSIZE     1034

int main()
{
    int fd1,fd3,fd4,fd5;
    int job1,job4;

    fd1=open(TTY1,O_RDWR);
    if(fd1<0)
    {
        perror("open()");
        exit(1);
    }
    write(fd1,"TTY1\n",5);
    fd3=open(TTY3,O_RDWR|O_NONBLOCK);
    if(fd3<0)
    {
        perror("open()");
        exit(1);
    }
    write(fd3,"TTY3\n",5);
    job1=rel_addjob(fd1,fd3);
    if(job1<0)
    {
        fprintf(stderr,"rel_addjob():%s\n",strerror(-job1));
        exit(1);
    }

    fd5=open(TTY5,O_RDWR);
    if(fd5<0)
    {
        perror("open()");
        exit(1);
    }
    write(fd5,"TTY5\n",5);
    fd4=open(TTY4,O_RDWR|O_NONBLOCK);
    if(fd4<0)
    {
        perror("open()");
        exit(1);
    }
    write(fd4,"TTY4\n",5);
    job4=rel_addjob(fd4,fd5);
    if(job4<0)
    {
         fprintf(stderr,"rel_addjob():%s\n",strerror(-job1));
        exit(1);
    }

    while(1)
    {
        pause();
    }

    close(fd1);
    close(fd3);
    close(fd4);
    close(fd5);
    exit(0);
}