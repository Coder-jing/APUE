#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

#define PNUM    20
#define FNAME   "/tmp/out"
#define BUFSIZE 1024

static void func_add(void);

int main()
{
    pid_t pid;
    for(int i=0;i<PNUM;i++)
    {   
        pid=fork();     
        if(pid<0)
        {
            fprintf(stderr,"fork()\n");
            exit(1);
        }
        if(pid==0)
        {
            func_add();
            exit(0);
        }
    }
     for(int i=0;i<PNUM;i++)
        {
            wait(NULL);
        }
    exit(0);
}

static void func_add(void)
{
    FILE* fp;
    char buf[BUFSIZE];

    fp=fopen(FNAME,"r+");
    if(fp==NULL)
    {
        perror("fopen()");
        exit(1);
    }

    int fd=fileno(fp);

    lockf(fd,F_LOCK,0);
    int i=atoi(fgets(buf,BUFSIZE,fp));
    fseek(fp,0,SEEK_SET);
    fprintf(fp,"%d\n",i+1);
    fflush(fp);
    lockf(fd,F_ULOCK,0);
    fclose(fp);
    return;
}