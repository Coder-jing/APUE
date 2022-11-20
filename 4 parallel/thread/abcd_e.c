#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>


#define TNUM  4

static void* thr_func(void* p);
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
static int num=0;

int main()
{
    pthread_t tid[TNUM];
    for(int i=0;i<TNUM;i++)
    {
        int err=pthread_create(tid+i,NULL,thr_func,(void*)i);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }

    alarm(2);

    for(int i=0;i<TNUM;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    exit(0);
}

static int next(int n)
{
    return n+1==4?0:n+1;
}

static void* thr_func(void* p)
{
    int n=(int)p;
    int x='a'+n;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(num!=n)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        write(1,&x,1);
        num=next(num);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}