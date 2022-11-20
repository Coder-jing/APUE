#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

#define     LEFT    30000000
#define     RIGHT   30000200
#define     THREADNUM   4

static void* primer_func(void* p);
pthread_mutex_t mutex_num=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
int num=0;

int main()
{
    pthread_t tid[THREADNUM];
    int err;
    for(int i=0;i<THREADNUM;i++)
    {
        err=pthread_create(tid+i,NULL,primer_func,(void*)i);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }


    for(int i=LEFT;i<=RIGHT;i++)
    {
        pthread_mutex_lock(&mutex_num);
        while(num!=0)
        {
            pthread_cond_wait(&cond,&mutex_num);
        }
        num=i;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex_num);
    }

    
    pthread_mutex_lock(&mutex_num);
    while(num!=0)
    {
        pthread_mutex_unlock(&mutex_num);
        sched_yield();
        pthread_mutex_lock(&mutex_num);
    }
    num=-1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex_num);


    for(int i=0;i<THREADNUM;i++)
    {
        pthread_join(tid[i],NULL);
    }

    pthread_mutex_destroy(&mutex_num);
    pthread_cond_destroy(&cond);
    exit(0);
}

static void* primer_func(void* p)
{
    while(1)
    {
        pthread_mutex_lock(&mutex_num);
        while(num==0)
        {
            pthread_cond_wait(&cond,&mutex_num);
        }
        if(num==-1)
        {
            pthread_mutex_unlock(&mutex_num);
            break;
        }
        int i=num;
        num=0;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex_num);

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
            printf("[%d]:%d is primer!\n",(int)p,i);
        }
    }
    pthread_exit(NULL);
}