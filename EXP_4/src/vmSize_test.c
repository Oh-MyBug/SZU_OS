#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

int main(){
	int i, MAX_NUM = 100000;
	char *m;
	printf("Process %d has been generated!\n", getpid());
	getchar();
	for(i = 1; i < MAX_NUM; i ++){
		m = (char *)malloc(i*pow(2,30)*sizeof(char)); // 分配1G内存
		if(m == NULL){
			printf("Run out of virtual memory!\n");
			getchar();
			break;
		}else
			free(m); // 释放空间
	}
	return 0;
}
