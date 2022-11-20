#include<stdio.h>
#include<stdlib.h>


int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage:%s <src_file>\n",argv[0]);
        exit(1);
    }
    FILE *fp;
    fp=fopen(argv[1],"r");
    if(fp==NULL)
    {
        perror("fopen()");
        exit(1);
    }
    fseek(fp,0,SEEK_END);
    printf("count=%d\n",ftell(fp));
    fclose(fp);
    exit(0);
}