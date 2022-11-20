#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<string.h>


#define BUFSIZE 1024
#define FMTSIZE 1024

/* 
    -y: year    -y 4|四位   -y 2|两位
    -m: month
    -d: day
    -H: hour    -H 12|12小时制  -H 24|24小时制
    -M: minute
    -S: second
 */

int main(int argc,char** argv)
{
    time_t stamp;
    struct tm* s_tm;
    int c;
    char fmtstr[FMTSIZE];
    char buf[BUFSIZ];
    FILE* fp=stdout;

    fmtstr[0]='\0';
    stamp=time(NULL);
    s_tm=localtime(&stamp);
    while(1)
    {
        c=getopt(argc,argv,"-y:mdH:MS");
        if(c<0)
        {
            //perror("getopt()");
            break;
        }
        switch(c)
        {
            case 1:
                if(fp==stdout)
                {
                    fp=fopen(argv[optind-1],"w");
                    if(fp==NULL)
                    {
                        perror("fopen()");
                        fp=stdout;
                    }
                }
            break;
            case 'y':
                if(strcmp(optarg,"4")==0)
                {
                    strncat(fmtstr,"%Y ",FMTSIZE);
                }
                else if(strcmp(optarg,"2")==0)
                {
                    strncat(fmtstr,"%y ",FMTSIZE);
                }
                else
                {
                    fprintf(stderr, "Invalid argment error\n");
                }
            break;
            case 'm':
                strncat(fmtstr,"%m ",FMTSIZE);
            break;
            case 'd':
                strncat(fmtstr,"%d ",FMTSIZE);
            break;
            case 'H':
                if(strcmp(optarg,"24")==0)
                {
                    strncat(fmtstr,"%H ",FMTSIZE);
                }
                else if(strcmp(optarg,"12")==0)
                {
                    strncat(fmtstr,"(%P)%I ",FMTSIZE);
                }
                else
                {
                    fprintf(stderr, "Invalid argment error\n");
                }
            break;
            case 'M':
                strncat(fmtstr,"%M ",FMTSIZE);
            break;
            case 'S':
                strncat(fmtstr,"%S ",FMTSIZE);
            break;
            default:
            break;
        }
        
    }
    strncat(fmtstr,"\n",FMTSIZE);
    strftime(buf,BUFSIZE,fmtstr,s_tm);
    fputs(buf,fp);
    
    if(fp!=stdout)
    {
        fclose(fp);
    }

    exit(0);
}
