#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<errno.h>
#include<sys/time.h>
#include"mytbf.h"


static struct mytbf_st* job[MYTBF_MAX];
static int inited=0;
static pthread_mutex_t mutex_job=PTHREAD_MUTEX_INITIALIZER;
static pthread_t tid_alrm;
static pthread_once_t init_once=PTHREAD_ONCE_INIT;

struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static int get_free_pos_unlocked(void)
{
    for(int i=0;i<MYTBF_MAX;i++)
    {
        if(job[i]==NULL)
        {
            return i;
        }
    }
    return -1;
}

static int min(int a,int b)
{
    if(a<b)
    {
        return a;
    }
    return b;
}

static void* thr_alrm(int s)
{

    while(1)
    {
        pthread_mutex_lock(&mutex_job);
        for(int i=0;i<MYTBF_MAX;i++)
        {
            if(job[i]!=NULL)
            {
                pthread_mutex_lock(&job[i]->mutex);
                job[i]->token+=job[i]->cps;
                if(job[i]->token>job[i]->burst)
                {
                    job[i]->token=job[i]->burst;
                }
                pthread_cond_broadcast(&job[i]->cond);
                pthread_mutex_unlock(&job[i]->mutex);
            } 
        }
        pthread_mutex_unlock(&mutex_job);
        sleep(1);
    }
    
}

static void module_unload(void)
{
   pthread_cancel(tid_alrm);
   pthread_join(tid_alrm,NULL);

    for(int i=0;i<MYTBF_MAX;i++)
    {
        if(job[i]!=NULL)
        {
            mytbf_destroy(job[i]);
        }
        free(job[i]);
    }
    pthread_mutex_destroy(&mutex_job);
}

static void module_load(void)
{
    
    int err=pthread_create(&tid_alrm,NULL,thr_alrm,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create():%s",strerror(err));
        exit(1);
    }

    atexit(module_unload);
}


mytbf_t* mytbf_init(int cps,int burst)
{
    struct mytbf_st *me;
    
    /* if(!inited)
    {
        module_load();
        inited=1;
    }
     */

    pthread_once(&init_once,module_load);

    me=malloc(sizeof(me));
    if(me==NULL)
    {
        return NULL;
    }
    me->token=0;
    me->cps=cps;
    me->burst=burst;
    pthread_mutex_init(&me->mutex,NULL);
    pthread_cond_init(&me->cond,NULL);
    pthread_mutex_lock(&mutex_job);
    int pos=get_free_pos_unlocked();
    if(pos<0)
    {
        pthread_mutex_unlock(&mutex_job);
        free(me);
        return NULL;
    }
    me->pos=pos;
    job[pos]=me;
    pthread_mutex_unlock(&mutex_job);
    return me;
}

int mytbf_fetchtoken(mytbf_t* ptr,int size)
{
    struct mytbf_st* me=ptr;
    if(size<=0)
    {
        return -EINVAL;
    }
    pthread_mutex_lock(&me->mutex);
    while(me->token<=0)
    {
        pthread_cond_wait(&me->cond,&me->mutex);
        /* pthread_mutex_unlock(&me->mutex);
        sched_yield();
        pthread_mutex_lock(&me->mutex); */
    }
    int n=min(me->token,size);
    me->token-=n;
    pthread_mutex_unlock(&me->mutex);
    return n;
}

int mytbf_returntoken(mytbf_t* ptr,int size)
{
    struct mytbf_st* me=ptr;
    if(size<=0)
    {
        return -EINVAL;
    }
    pthread_mutex_lock(&me->mutex);
    me->token+=size;
    if(me->token>me->burst)
    {
        me->token=me->burst;
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mutex);
    return size;
}

int mytbf_destroy(mytbf_t* ptr)
{
    struct mytbf_st* me=ptr;
    pthread_mutex_lock(&mutex_job);
    job[me->pos]=NULL;
    pthread_mutex_unlock(&mutex_job);
    pthread_mutex_destroy(&me->mutex);
    pthread_cond_destroy(&me->cond);
    free(ptr);

    return 0;
}