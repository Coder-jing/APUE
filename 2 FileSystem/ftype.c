#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

static int ftype(char* filename)
{
    struct stat statres;
    int res=stat(filename,&statres);
    if(res<0)
    {
        perror("stat()");
        exit(1);
    }
    if(S_ISREG(statres.st_mode))
    {
        return '-';
    }
    else if(S_ISDIR(statres.st_mode))
    {
        return 'd';
    }
    else if(S_ISCHR(statres.st_mode))
    {
        return 'c';
    }
    else if(S_ISFIFO(statres.st_mode))
    {
        return 'p';
    }
    else if(S_ISLNK(statres.st_mode))
    {
        return 'l';
    }
    else if(S_ISSOCK(statres.st_mode))
    {
        return 's';
    }
    else if(S_ISBLK(statres.st_mode))
    {
        return 'b';
    }

    return 0;
}


int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    // ftype(argv[1]);
    printf("type=%c\n",(char)ftype(argv[1]));
    exit(0);
}