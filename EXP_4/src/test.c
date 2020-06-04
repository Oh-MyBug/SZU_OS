/*************************************************************************
    > File Name: test.c
    > Author: C.Li
    > Mail: justleecong@163.com 
    > Created Time: 2020年06月03日 星期三 08时42分00秒
 ************************************************************************/

#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
	int *p;
	printf("process %d\n",getpid());
	getchar();
	
	p = (int *) malloc(4000000000000000);
	if(p != NULL){
		printf("Success!\n");
	}else{
		printf("Failed!\n");
	}
	getchar();

	free(p);
	printf("Release!\n");
	getchar();
	return 0;
}
