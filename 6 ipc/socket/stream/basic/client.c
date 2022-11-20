#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>

#include"proto.h"
#define IPSTRSIZE   40


int main(int argc,char* argv[])
{
    struct sockaddr_in raddr;
    socklen_t raddr_len;
    long long stamp;


    int sd=socket(AF_INET,SOCK_STREAM,0);
    if(sd<0)
    {
        perror("socket()");
        exit(0);
    }

    //bind()
    raddr.sin_family=AF_INET;
    raddr.sin_port=htons(atoi(SERVERPORT));
    inet_pton(AF_INET,argv[1],&raddr.sin_addr);
    if(connect(sd,(void*)&raddr,sizeof(raddr))<0)
    {
        perror("connect()");
        exit(1);
    }

    FILE* fp=fdopen(sd,"r+");
    if(fp==NULL)
    {
        perror("fdopen()");
        exit(1);
    }

    if(fscanf(fp,FMT_STAMP,&stamp)<1)
    {
        fprintf(stderr,"Bad format!\n");
    }
    else
    {
        fprintf(stdout,"stamp=%lld\n",stamp);
    }
    //recv()
    //close()
    fclose(fp);


    close(sd);


    exit(0);
}