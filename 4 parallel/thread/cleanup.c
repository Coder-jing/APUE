#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

static void cleanupfunc(void* p)
{
    puts(p);
}

static void* func(void* p)
{
    puts("Thread is working");

    pthread_cleanup_push(cleanupfunc,"cleanup:1");
    pthread_cleanup_push(cleanupfunc,"cleanup:2");
    pthread_cleanup_push(cleanupfunc,"cleanup:3");

    puts("push over!");

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    int err;

    puts("Begin");

    err=pthread_create(&tid,NULL,func,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_creare():%s\n",strerror(err));
    }

    puts("End");

    pthread_join(tid,NULL);
    exit(0);
}