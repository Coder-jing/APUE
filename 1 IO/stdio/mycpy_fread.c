#include<stdio.h>
#include<stdlib.h>

#define SIZE 1024

void syserr(char* message)
{
    perror(message);
    exit(1);
}

int main(int argc,char** argv)
{
    if(argc<3)
    {
        fprintf(stderr,"Usage:%s <src_file> <des_file>\n",argv[0]);
        exit(1);
    }
    FILE* src=NULL;
    FILE* des=NULL;
    char buffer[SIZE];
    src=fopen(argv[1],"r");
    if(src==NULL)
    {
        syserr("fopen()");
    }
    des=fopen(argv[2],"w");
    if(des==NULL)
    {
        syserr("fopen()");
    }
    int n;
    while((n=fread(buffer,1,SIZE,src)))
    {
        fwrite(buffer,1,n,des);
    }
    fclose(des);
    fclose(src);
    printf("copy done!\n");
    exit(0);
}