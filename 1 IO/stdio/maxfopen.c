#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main()
{
    int count=0;
    FILE *fp=NULL;
    while(1)
    {
        fp=fopen("tmp.txt","r");
        if(fp==NULL)
        {   
        
            //fprintf(stderr,"fopen() failed,errno=%d\n",errno);
            perror("fopen()");
           //fprintf(stderr,"fopen():%s\n",strerror(errno));
           break;
        }
        //puts("OK!\n");
        count++;
    }
    printf("count=%d\n",count);
    exit(0);
}