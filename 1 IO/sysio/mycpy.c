#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>


#define BUFSIZE 1024


int main(int argc,char** argv)
{
    if(argc<3)
    {
        fprintf(stderr,"Usage:%s <src_file> <des_file>\n",argv[0]);
        exit(1);
    }
    int fds;
    int fdd;
    fds=open(argv[1],O_RDONLY);
    if(fds<0)
    {
        perror("fopen()");
        exit(1);
    }
    fdd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(fdd<0)
    {
        perror("fopen()");
        close(fds);
        exit(1);
    }
    char buf[BUFSIZE];
    int len;
    int ret;
    while(1)
    {
        len=read(fds,buf,BUFSIZE);
        if(len<0)
        {
            perror("read()");
            break;
        }
        if(len==0)
        {
            break;
        }
        int pos=0;
        while(len>0)   //保证读出的数据完全写入
        {
            ret=write(fdd,buf+pos,len);
            if(ret<0)
            {
                perror("write()");
                break;
            }
            pos+=ret;
            len-=ret;
        }
    }

    close(fdd);
    close(fds);
    printf("copy done!\n");
    exit(0);
}
