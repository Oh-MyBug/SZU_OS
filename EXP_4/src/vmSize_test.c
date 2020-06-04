#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

int main(){
	int MAX_NUM = 100000;
	char *m[MAX_NUM];
	printf("Process %d has been generated!\n", getpid());
	getchar();
	for(int i = 0; i < MAX_NUM; i ++){
		p[i] = (char *)malloc(pow(2,30)*sizeof(char)); // 分配1G内存
		if(p[i] == NULL){
			printf("Run out of virtual memory!\n");
			getchar();
			break;
		}
	}
	return 0;
}