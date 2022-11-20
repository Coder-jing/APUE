#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

static void int_handler(int a)
{
    write(1,"!",1);
}

int main()
{
    sigset_t sig,oset,saveset;
    sigemptyset(&sig);
    sigaddset(&sig,SIGINT);
    signal(SIGINT,int_handler);
    sigprocmask(SIG_UNBLOCK,&sig,&saveset);//  保存状态
    while(1)
    {
        sigprocmask(SIG_BLOCK,&sig,&oset);
        for(int i=0;i<5;i++)
        {
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
        //sigprocmask(SIG_UNBLOCK,&sig,NULL);
        sigprocmask(SIG_SETMASK,&oset,NULL);
    }
    sigprocmask(SIG_SETMASK,&saveset,NULL); //恢复初始状态
    exit(0);
}