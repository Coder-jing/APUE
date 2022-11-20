#include<stdio.h>
#include<stdlib.h>
#include<glob.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>



#define DLIMS " \t\n"
struct cmd_t
{
    glob_t globres;
};

static void prompt(void)
{
    printf("mybash-0$: ");
}

static void parse( char* line,struct cmd_t* cmd)
{
    char* tok;
    int i=0;
    while(1)
    {
        tok=strsep(&line,DLIMS);
        if(tok==NULL)
        {
            break;
        }
        if(tok[0]=='\0')
        {
            continue;
        }
        glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&(cmd->globres));
        i=1;
    }
}
int main()
{
    char* linebuf=NULL;
    size_t linebuf_size=0;
    struct cmd_t cmd;
    pid_t pid;

    while(1)
    {
        prompt();

        if(getline(&linebuf,&linebuf_size,stdin)<0)
        {
            break;
        }
        glob_t globres;
        parse(linebuf,&cmd);

        if(0)    //内部命令
        {
            //do sth
        }
        else    //外部命令
        {
            pid=fork();
            if(pid<0)
            {
                perror("fork()");
                exit(1);
            }
            if(pid==0)  //child
            {
                execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
                perror("execvp()");
                exit(1);
            }
            else
            {
                wait(NULL);
            }
        }
    }
    exit(0);
}