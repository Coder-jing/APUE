#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>


#define TTY1    "/dev/tty11"
#define TTY3    "/dev/tty33"

#define BUFSIZE     1034

enum
{
    STATE_R=1,
    STATE_W,
    STATE_Ex,
    STATE_T
};

struct fsm_st
{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int len;
    int pos;
    char* errorstr;
};

static void fsm_driver(struct fsm_st* fsm)
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
static void relay(int fd1,int fd3)
{
    int fd1_save,fd3_save;
    struct fsm_st fsm13,fsm31;

    fd1_save=fcntl(fd1,F_GETFL);
    fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);

    fd3_save=fcntl(fd3,F_GETFL);
    fcntl(fd3,F_SETFL,fd3_save|O_NONBLOCK);

    fsm13.state=STATE_R;
    fsm13.sfd=fd1;
    fsm13.dfd=fd3;
    fsm31.state=STATE_R;
    fsm31.sfd=fd3;
    fsm31.dfd=fd1;

    while(fsm13.state!=STATE_T||fsm31.state!=STATE_T)
    {
        fsm_driver(&fsm13);
        fsm_driver(&fsm31);
    }
    fcntl(fd1,F_SETFL,fd1_save);
    fcntl(fd3,F_SETFL,fd3_save);

}

int main()
{
    int fd1,fd3;
    fd1=open(TTY1,O_RDWR);
    if(fd1<0)
    {
        perror("open()");
        exit(1);
    }
    fd3=open(TTY3,O_RDWR|O_NONBLOCK);
    if(fd3<0)
    {
        perror("open()");
        exit(1);
    }

    relay(fd1,fd3);

    close(fd1);
    close(fd3);
    exit(0);
}