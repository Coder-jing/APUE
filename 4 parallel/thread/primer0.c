#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

#define     LEFT    30000000
#define     RIGHT   30000200
#define     THREADNUM   RIGHT-LEFT+1

static void* primer_func(void* p);

int main()
{
    pthread_t tid[THREADNUM];
    int err;
    for(int i=LEFT;i<=RIGHT;i++)
    {
        err=pthread_create(tid+i-LEFT,NULL,primer_func,(void*)i);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }

    for(int i=LEFT;i<=RIGHT;i++)
    {
        pthread_join(tid[i-LEFT],NULL);
    }

    exit(0);
}

static void* primer_func(void* p)
{
    int i=(int)p;
    int mark=1;
    for(int j=2;j<i/2;j++)
    {
        if(i%j==0)
        {
            mark=0;
            break;
        }
    }
    if(mark)
    {
        printf("%d is primer!\n",i);
    }
    pthread_exit(NULL);
}