#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

static off_t flen(char* filename)
{
    struct stat statres;
    int fs=stat(filename,&statres);
    if(fs<0)
    {
        perror("stat()");
        exit(-1);
    }
    return statres.st_size;
}

int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage:%s <filename> \n",argv[0]);
        exit(1);
    }
   
   printf("size=%lld\n", (long long)flen(argv[1]));
   exit(0);
}