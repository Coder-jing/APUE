#include<stdio.h>
#include<stdlib.h>

int main()
{
    extern char** environ;

    // for(int i=0;environ[i]!=NULL;i++)
    // {
    //     puts(environ[i]);
    // }
    char* s=getenv("PATH");
    puts(s);
    
    exit(0);
}