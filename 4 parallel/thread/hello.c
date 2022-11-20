#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
#pragma omp parallel
	{
		puts("hello");
		puts("world");
	}
	exit(0);
}

