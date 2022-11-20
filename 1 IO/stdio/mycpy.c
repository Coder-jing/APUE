#include<stdio.h>
#include<stdlib.h>

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
    int c;
    while((c=fgetc(src))!=EOF)
    {
        fputc(c,des);
    }
    fclose(des);
    fclose(src);
    printf("copy done!\n");
    exit(0);
}