#include<stdlib.h>
#include<stdio.h>
#include<pwd.h>
#include<sys/types.h>

int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    struct passwd* pd;
    pd=getpwnam(argv[1]);
    printf("name=%s\n",pd->pw_name);
    printf("uid=%d\n",pd->pw_uid);
    exit(0);
}