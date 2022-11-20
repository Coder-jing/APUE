#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv)
{
    FILE *fp1;
    fp1=fopen(argv[1],"r");
    if(fp1==NULL)
    {
        perror("open()");
        exit(1);
    }
    fseek(fp1,0,SEEK_END);
    
    printf("%d\n",ftell(fp1));
}