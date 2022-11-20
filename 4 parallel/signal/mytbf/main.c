#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<string.h>
#include"mytbf.h"


#define CPS     20
#define BUFSIZE 1024

#define BURST   100


int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    
    mytbf_t* tbf=mytbf_init(CPS,BURST);
    if(tbf==NULL)
    {
        fprintf(stderr,"mytbf_init() failed!\n");
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
        int size=mytbf_fetchtoken(tbf,BUFSIZE);
        if(size<0)
        {
            fprintf(stderr,"mytbf_fetchtoken():%s",strerror(-size));
            exit(1);
        }

        while((len=read(sfd,buf,size))<0)
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
        
        int pos=0;
        int ret=0;
        if(size-len>0)
        {
            mytbf_returntoken(tbf,size-len);
        }
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
    mytbf_destroy(tbf);
    exit(0);
}