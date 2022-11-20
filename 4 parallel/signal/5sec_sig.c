#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

int64_t count=0;
volatile int loop=1;

static void sig_handler(int s)
{
    loop=0;
}

int main()
{
    alarm(5);
    signal(SIGALRM,sig_handler);
    while(loop)
    {
        count++;
    }

    printf("%ld",count);
    exit(0);
}