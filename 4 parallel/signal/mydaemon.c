#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<syslog.h>
#include<errno.h>
#include<string.h>
#include<signal.h>



#define FNAME "/tmp/out"
FILE* fp;

static int daemonlize(void)
{
    pid_t pid;
    int fd;
    pid=fork();
    if(pid<0)
    {
        //perror("fork()");
        return -1;
    }
    if(pid>0)
    {
        exit(0);
    }
    fd=open("/dev/null",O_RDWR);
    if(fd<0)
    {
        //perror("open()");
        return -1;
    }
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    if(fd>2)
    {
        close(fd);
    }
    //umask(0);
    setsid();
    chdir("/");
    
    return 0;

}

static void daemon_exit(int s)
{
    closelog();
    fclose(fp);
    exit(0);
}

int main()
{
    pid_t pid;

    struct sigaction sa;
    sa.sa_handler=daemon_exit;
    sa.sa_flags=0;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGINT);
    sigaddset(&sa.sa_mask,SIGTERM);
    sigaddset(&sa.sa_mask,SIGQUIT);

    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGQUIT,&sa,NULL);
    sigaction(SIGTERM,&sa,NULL);
    
    openlog("mydaemon",LOG_PID,LOG_DAEMON);
    if(daemonlize()<0)
    {
        syslog(LOG_ERR,"demonzied() failed!");
        exit(1);
    }
    else
    {
        syslog(LOG_INFO,"daemonized succeed!");
    }

    fp=fopen(FNAME,"w");
    if(fp==NULL)
    {
        //perror("fopen()");
        syslog(LOG_ERR,"fopen() faild! %s",strerror(errno));
        exit(1);
    }
    syslog(LOG_INFO, "%s is opened!", FNAME);
    for(int i=0;;i++)
    {
        fprintf(fp,"%d\n",i);
        fflush(fp);
        syslog(LOG_DEBUG,"%d is printed!",i);
        sleep(1);
    }

    

    exit(0);
}