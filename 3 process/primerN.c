#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define     LEFT    30000000
#define     RIGHT   30000200
#define     N       3

int main()
{
    pid_t pid;
    for(int n=0;n<N;n++)
    {
        pid=fork();
        if(pid<0)
        {
            perror("fork()");
            for(int i=0;i<n;i++)
            {
                wait(NULL);
            }
            exit(1);
        }
        if(pid==0)
        {
            for(int i=LEFT+n;i<=RIGHT;i+=N)
            {
                int mark=1;
                for(int j=2;j<i;j++)
                {
                    if(i%j==0)
                    {
                        mark=0;
                        break;
                    }
                }
                if(mark)
                    printf("[%d] %d is a primer!\n",n,i);
            }
            exit(0);
        }
    }

    for(int n=0;n<N;n++)
    {
        wait(NULL);
    }
    exit(0);
}