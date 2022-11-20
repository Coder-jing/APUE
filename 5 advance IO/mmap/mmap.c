#include<sys/mman.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc,char* argv[])
{
    if(argc<2)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    struct stat statres;
    char* str;
    int64_t count=0;

    int fd=open(argv[1],O_RDONLY);
    if(fd<0)
    {
        perror("open()");
        exit(1);
    }

    int err=fstat(fd,&statres);
    if(err<0)
    {
        perror("fstat()");
        exit(1);
    }
    str=mmap(NULL,statres.st_size,PROT_READ,MAP_SHARED,fd,0);
    if(str==NULL)
    {
        perror("mmap()");
        exit(1);
    }
    close(fd);
    
    for(int i=0;i<statres.st_size;i++)
    {
        if(str[i]=='a')
        {
            count++;
        }
    }
    printf("count=%ld\n",count);

    munmap(str,statres.st_size);

    exit(0);

}