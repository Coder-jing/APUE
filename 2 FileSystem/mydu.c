#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<glob.h>


#define PATHSIZE 1024

static int path_noloop(const char* path)
{
    char* pos;
    pos=strrchr(path,'/');
    if(pos==NULL)
    {
        exit(1);
    }
    if(strcmp(pos+1,".")==0||strcmp(pos+1,"..")==0)
    {
        return 0;
    }
    return 1;
}

int errorunc(const char* errorMsg,int errorNo)
{
    printf("%s\n",errorMsg);
    fprintf(stderr,"ERROR MSG:%s\n",strerror(errorNo));
    exit(1);
} 

static int64_t mydu(const char* path)
{

    static struct stat statres;
    glob_t globres;
    int i;
    int64_t sum;
    static char nextpath[PATHSIZE];

    int err=lstat(path,&statres);
    if(err<0)
    {
        perror("lstat()");
        exit(1);
    }
    //1.为文件
    if(!S_ISDIR(statres.st_mode))
    {
        fprintf(stdout,"%ld\t%s\n",statres.st_blocks/2,path);
        return statres.st_blocks;
    }

    strncpy(nextpath,path,PATHSIZE);
    //strncat(nextpath,"/*",PATHSIZE);
    strncat(nextpath,"/*",PATHSIZE-1);
    nextpath[PATHSIZE-1]='\0';
    int gerr=glob(nextpath,0,*errorunc,&globres);
    //int gerr=glob(nextpath,0,NULL,&globres);
    if(gerr<0)
    {
        perror("glob()");
        exit(1);
    }

    strncpy(nextpath,path,PATHSIZE);
    //strncat(nextpath,"/.*",PATHSIZE);
    strncat(nextpath,"/.*",PATHSIZE-1);
    nextpath[PATHSIZE-1]='\0';
    gerr=glob(nextpath,GLOB_APPEND,*errorunc,&globres);
    //gerr=glob(nextpath,GLOB_APPEND,NULL,&globres);
    if(gerr<0)
    {
        perror("glob()");
        exit(1);
    }

    sum=statres.st_blocks;
    for(i=0;i<globres.gl_pathc;i++)
    {
        if(path_noloop(globres.gl_pathv[i]))
        {
            sum+=mydu(globres.gl_pathv[i]);
        }      
    }
    globfree(&globres);
    return sum;
}


int main(int argc,char** argv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    int64_t len=mydu(argv[1])/2;
    printf("%ld\t%s\n",len,argv[1]);
    exit(0);
}