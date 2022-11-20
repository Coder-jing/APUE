#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

#define TNUM    20
#define FNAME   "/tmp/out"
#define BUFSIZE 1024

static void* thr_add(void* p);
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_t tid[TNUM];
    for(int i=0;i<TNUM;i++)
    {
        int err=pthread_create(tid+i,NULL,thr_add,NULL);
        if(err)
        {
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }
    for(int i=0;i<TNUM;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    exit(0);
}

static void* thr_add(void* p)
{
    FILE* fp;
    char buf[BUFSIZE];

    fp=fopen(FNAME,"r+");
    if(fp==NULL)
    {
        perror("fopen()");
        exit(1);
    }
    pthread_mutex_lock(&mutex);
    int i=atoi(fgets(buf,BUFSIZE,fp));
    fseek(fp,0,SEEK_SET);
    fprintf(fp,"%d\n",i+1);

    fclose(fp);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}