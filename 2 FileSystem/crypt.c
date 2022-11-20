#define _XOPEN_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<shadow.h>
#include<crypt.h>
#include<unistd.h>
#include<string.h>


int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage:%s <filename> \n",argv[0]);
        exit(1);
    }

    struct spwd *pd;
    char* shadow_pass;
    char* crypted;

    shadow_pass=getpass("PassWord:");
    pd=getspnam(argv[1]);
    crypted=crypt(shadow_pass,pd->sp_pwdp);
    if(strcmp(crypted,pd->sp_pwdp)==0)
    {
        puts("OK!\n");
    }
    else
    {
        puts("Failed!\n");
    }


    exit(0);
}
