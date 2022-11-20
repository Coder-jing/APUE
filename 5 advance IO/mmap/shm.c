#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define     MEMSIZE     1024

int main()
{
    char* str;
    pid_t pid;

    str=mmap(NULL,MEMSIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if(str==NULL)
    {
        perror("mmap()");
        exit(1);
    }

    pid=fork();
    if(pid<0)
    {
        perror("fork()");
        munmap(str,MEMSIZE);
        exit(1);
    }
    if(pid==0)
    {
        strcpy(str,"HELLO WORLD!");
        munmap(str,MEMSIZE);
        exit(0);
    }
    else
    {
        wait(NULL);
        puts(str);
        munmap(str,MEMSIZE);
        exit(0);
    }
}