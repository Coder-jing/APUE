#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

static void* func(void* p)
{
    puts("Thread is working");
    pthread_exit(NULL);
    //return NULL;
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