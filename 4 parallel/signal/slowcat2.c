#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>


#define CPS     10
#define BUFSIZE CPS

#define BURST   100

static volatile sig_atomic_t TOKEN=0;
static int LOOP=0;
static void sig_handler(int s)
{
    alarm(1);
    TOKEN++;
    if(TOKEN>BURST)
    {
        TOKEN=BURST;
    }
}

int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    char buf[BUFSIZE];
    int sfd;

    do
    {
        sfd=open(argv[1],O_RDONLY);
        if(sfd<0)
        {
            if(errno!=EINTR)
            {
                perror("open()");
                exit(1);
            }
        }
    }while(sfd<0);


    int len=0;
    while(1)
    {
        while((len=read(sfd,buf,BUFSIZE))<0)
        {  
            if(errno==EINTR)
                continue; 
            perror("read()");
            exit(1);
        }
        if(len==0)
        {
            break;
        }
        

        signal(SIGALRM,sig_handler);
        alarm(1);
        while(!TOKEN)
        {
            pause();
        }
        TOKEN--;
        int pos=0;
        int ret=0;
        while(len>0)
        {
            ret=write(1,buf+pos,len);
            if(ret<0)
            {
                if(errno==EINTR)
                continue; 
                perror("write()");
                exit(1);
            }
            pos+=ret;
            len-=ret;
        }
    }
    close(sfd);
    exit(0);
}