#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

#define     LEFT    30000000
#define     RIGHT   30000200
#define     THREADNUM   RIGHT-LEFT+1

struct thread_arg_st
{
    int n;
};

static void* primer_func(void* p);

int main()
{
    pthread_t tid[THREADNUM];
    int err;
    struct thread_arg_st *p;
    void* ptr;

    for(int i=LEFT;i<=RIGHT;i++)
    {
        p=malloc(sizeof(*p));
        if(p==NULL)
        {
            perror("malloc()");
            exit(1);
        }
        p->n=i;
        err=pthread_create(tid+i-LEFT,NULL,primer_func,p);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }

    for(int i=LEFT;i<=RIGHT;i++)
    {
        pthread_join(tid[i-LEFT],&ptr);
        free(ptr);
    }

    exit(0);
}

static void* primer_func(void* p)
{
    int i=((struct thread_arg_st*)p)->n;
    //free(p);
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
    pthread_exit(p);
}