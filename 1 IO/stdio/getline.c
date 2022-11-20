#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char** argv)
{
	if(argc<2)
	{
		fprintf(stderr,"Usage:%s <src_file> \n",argv[0]);
		exit(1);
	}
	FILE* fp;
	fp=fopen(argv[1],"r");
	if(fp==NULL)
	{
		perror("fopen()");
		exit(1);
	}
	char* buf=NULL;
	int bufSize=0;
	while(getline(&buf,&bufSize,fp)>0)
	{
		printf("size=%d\n",bufSize);
		printf("len=%d\n",strlen(buf));
	}
	fclose(fp);
	exit(0);
	}

