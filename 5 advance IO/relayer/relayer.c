#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include"relayer.h"

#define BUFSIZE 1000
#define REL_JOBMAX  10000

static struct rel_job_st* rel_job[REL_JOBMAX];
static pthread_mutex_t mut_rel_job=PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once=PTHREAD_ONCE_INIT; 
enum
{
    STATE_R=1,
    STATE_W,
    STATE_Ex,
    STATE_T
};

struct rel_fsm_st
{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int len;
    int pos;
    char* errorstr;
    int64_t count;
};

struct rel_job_st
{
    int job_state;
    int fd1;
    int fd3;
    struct rel_fsm_st fsm13,fsm31;
    int fd1_save,fd3_save;
    //struct timerval start,end;  
};

static void fsm_driver(struct rel_fsm_st* fsm)
{
    switch (fsm->state)
    {
        case STATE_R:
            fsm->len=read(fsm->sfd,fsm->buf,BUFSIZE);
            if(fsm->len==0)
            {
                fsm->state=STATE_T;
            }
            else if(fsm->len<0)
            {
                if(errno==EAGAIN)
                {
                    fsm->state=STATE_R;
                }
                else
                {
                    fsm->errorstr="read()";
                    fsm->state=STATE_Ex;
                }
            }
            else
            {
                fsm->pos=0;
                fsm->state=STATE_W;
            }
            break;
        case STATE_W:
            int ret=write(fsm->dfd,fsm->buf+fsm->pos,fsm->len);
            if(ret<0)
            {
                if(errno==EAGAIN)
                {
                    fsm->state=STATE_W;
                }
                else
                {
                    fsm->errorstr="write()";
                    fsm->state=STATE_Ex;
                }
            }
            else
            {
                fsm->pos+=ret;
                fsm->len-=ret;
                if(fsm->len==0)
                {
                    fsm->state=STATE_R;
                }
                else
                {
                    fsm->state=STATE_W;
                }
            }
            break;
        case STATE_Ex:
            perror(fsm->errorstr);
            fsm->state=STATE_T;
            break;
        case STATE_T:
            /* do  something */
            break;
        default:
             /* do  something */
             abort();
            break;
    }
}

static void* thr_relayer(void* p) 
{
    while(1)
    {
        pthread_mutex_lock(&mut_rel_job);
        for(int i=0;i<REL_JOBMAX;i++)
        {
            if(rel_job[i]!=NULL)
            {
                if(rel_job[i]->job_state==STATE_RUNNING)
                {
                    fsm_driver(&rel_job[i]->fsm13);
                    fsm_driver(&rel_job[i]->fsm31);
                    if(rel_job[i]->fsm13.state==STATE_T&&rel_job[i]->fsm31.state==STATE_T)
                    {
                        rel_job[i]->job_state=STATE_OVER;
                    }
                    
                }
            }
        }
        pthread_mutex_unlock(&mut_rel_job);
    }
}
static void module_load(void)
{
    pthread_t tid_relayer;
    int err=pthread_create(&tid_relayer,NULL,thr_relayer,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create():%s\n",strerror(err));
        exit(1);
    }
}

static int get_free_pos_unlocked()
{
    for(int i=0;i<REL_JOBMAX;i++)
    {
        if(rel_job[i]==NULL)
        {
            return i;
        }
    }
    return -1;
}
int rel_addjob(int fd1,int fd3)
{
    struct rel_job_st* me;
    
    pthread_once(&init_once,module_load);

    me=malloc(sizeof(*me));
    if(me==NULL)
    {
        return -ENOMEM;
    }
    me->fd1=fd1;
    me->fd3=fd3;
    me->job_state=STATE_RUNNING;

    me->fd1_save=fcntl(me->fd1,F_GETFL);
    fcntl(me->fd1,F_SETFL,me->fd1_save|O_NONBLOCK);
    me->fd3_save=fcntl(me->fd3,F_GETFL);
    fcntl(me->fd3,F_SETFL,me->fd3_save|O_NONBLOCK);

    me->fsm13.sfd=me->fd1;
    me->fsm13.dfd=me->fd3;
    me->fsm13.state=STATE_R;

    me->fsm31.sfd=me->fd3;
    me->fsm31.dfd=me->fd1;
    me->fsm31.state=STATE_R;

    pthread_mutex_lock(&mut_rel_job);
    int pos=get_free_pos_unlocked();
    if(pos<0)
    {
        pthread_mutex_unlock(&mut_rel_job);
        fcntl(me->fd1,F_SETFL,me->fd1_save);
        fcntl(me->fd3,F_SETFL,me->fd3_save);
        free(me);
        return -ENOSPC;
    }
    rel_job[pos]=me;

    pthread_mutex_unlock(&mut_rel_job);

    return pos;
}
#if 0
int rel_canceljob(int id)
{

}
int rel_waitjob(int id,struct rel_stat_st* ptr)
{

}

int rel_statjob(int id,struct rel_stat_st* ptr)
{

}
#endif