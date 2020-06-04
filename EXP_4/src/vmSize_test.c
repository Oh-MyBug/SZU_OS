#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

int main(){
	int i, MAX_NUM = 100000000;
	char *m;
	printf("Process %d has been generated!\n", getpid());
	getchar();
	for(i = 1024; i < MAX_NUM; i ++){
		m = (char *)malloc(i*pow(2,20)*sizeof(char));
		if(m == NULL){
			printf("Allocating %dMB memory, run out of virtual memory!\n", i);
			getchar();
			break;
		}else
			free(m); // 释放空间
	}
	return 0;
}
