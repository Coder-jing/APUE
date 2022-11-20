#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define     LEFT    30000000
#define     RIGHT   30000200

int main()
{
    pid_t pid;
    for(int i=LEFT;i<=RIGHT;i++)
    {
        pid=fork();
        if(pid<0)
        {
            perror("fork()");
            exit(1);
        }
        if(pid==0)
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
            {
                printf("%d is a primer!\n",i);
            }
            exit(0);
        }
    }
    int st;
    for(int i=LEFT;i<=RIGHT;i++)
    {
        //wait(&st);
        wait(NULL);
    }
    exit(0);
}