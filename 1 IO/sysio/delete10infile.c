#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFSIZE 1024

int getLine(FILE* fp,int line)
{
    if(fp==NULL)
    {
        return -1;
    }
    fseek(fp,0,SEEK_SET);
    char ch;
    int i=1;
    while(!(line==i))
    {
        ch=fgetc(fp);
        if(ch=='\n')
        {
            i++;
        }
        if(ch==EOF)
        {
            return -1;
        }
    }
    return 0;
}

int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage:%s <filename>\n",argv[0]);
        exit(1);
    }
    FILE *fp1,*fp2;
    fp1=fopen(argv[1],"r");
    if(fp1==NULL)
    {
        perror("open()");
        exit(1);
    }
    fp2=fopen(argv[1],"r+");
    if(fp2==NULL)
    {
        perror("open()");
        fclose(fp1);
        exit(1);
    }
    getLine(fp1,11);
    getLine(fp2,10);
    off_t o1=ftell(fp1);
    printf("o1=%d\n",(int)o1);
    off_t o2=ftell(fp2);
    printf("o2=%d\n",(int)o2);
    int len,ret;
    char buf[BUFSIZE];
    while(1)
    {
        len=fread(buf,1,BUFSIZE,fp1);
        if(len<0)
        {
            perror("read()");
            exit(1);
        }
        if(len==0)
        {
            break;
        }
        int pos=0;
        while(len>0)
        {
            ret=fwrite(buf+pos,1,len,fp2);
            if(ret<0)
            {
                perror("write()");
                exit(1);
            }
            pos+=ret;
            len-=ret;
        }
    }
    off_t o3=ftell(fp1);
    printf("o3=%d\n",(int)o3);
    off_t o4=o3-o1+o2;
    printf("o4=%d\n",(int)o4);
    fclose(fp2);
    fclose(fp1);
    truncate("tmp.txt",o4);
    printf("modified!\n");
    exit(0);
}