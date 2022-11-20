#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include"mysem.h"

#define     LEFT    30000000
#define     RIGHT   30000200
#define     THREADNUM   RIGHT-LEFT+1
#define     N       4

static void* primer_func(void* p);
static mysem_t *sem;

int main()
{
    pthread_t tid[THREADNUM];
    int err;

    
    sem=mysem_init(N);
    if(sem==NULL)
    {
        fprintf(stderr,"mysem_init() failed!\n");
        exit(1);
    }
    for(int i=LEFT;i<=RIGHT;i++)
    {
        mysem_sub(sem,1);
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

    mysem_destroy(sem);

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
    sleep(5);
    mysem_add(sem,1);
    pthread_exit(NULL);
}