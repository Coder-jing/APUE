#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

#define FNAME "/tmp/out"
#define BUFSIZE 1024

int main(int argc,char** argv)
{
    FILE* fp;
    time_t stamp;
    struct tm* s_tm;
    char buf[BUFSIZE];
    int count=0;

    fp=fopen(FNAME,"a+");
    if(fp==NULL)
    {
        perror("fopen()");
        exit(1);
    }

    
    while(fgets(buf,BUFSIZE,fp)!=NULL)
    {
        count++;
    }
    while(1)
    {
        time(&stamp);
        s_tm=localtime(&stamp);

        fprintf(fp,"%-4d %d-%d-%d %d:%d:%d\n",count,s_tm->tm_year+1900,
        s_tm->tm_mon+1,s_tm->tm_mday,s_tm->tm_hour,
        s_tm->tm_min,s_tm->tm_sec);
        count++;
        
        fflush(fp);
        sleep(1);
    }
    fclose(fp);
    exit(0);
}