#include<stdio.h>
#include<stdlib.h>
#include<glob.h>
#include<string.h>

#define PAT "/etc/s*.conf"

int errfunc (const  char *epath, int eerrno)
{
    printf("%s\n",epath);
    fprintf(stderr,"ERROR MSG:%s\n",strerror(eerrno));
    return 0;
}

int main(int argc,char** argv)
{

    glob_t globres;
    int err=glob(PAT,0,*errfunc,&globres);
    if(err)
    {
        perror("aglob()");
        exit(1);
    }
    for(int i=0;i<globres.gl_pathc;i++)
    {
        printf("%s\n",globres.gl_pathv[i]);
    }
    globfree(&globres);
    exit(0);
}