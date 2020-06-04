#include<unistd.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main(){
	printf("Process %d has been generated!\n", getpid());
	getchar();
	
	int *m1,*m2,*m3,*m4,*m5,*m6,*m7,*m8;
	
	// 连续申请分配六个128MB空间（记为1~6号）
	m1 = (int *) malloc(pow(2,25)*sizeof(int));
	m2 = (int *) malloc(pow(2,25)*sizeof(int));
	m3 = (int *) malloc(pow(2,25)*sizeof(int));
	m4 = (int *) malloc(pow(2,25)*sizeof(int));
	m5 = (int *) malloc(pow(2,25)*sizeof(int));
	m6 = (int *) malloc(pow(2,25)*sizeof(int));
	
	if(m1 != NULL && m2 != NULL && m3 != NULL && m4 != NULL && m5 != NULL && m6 != NULL)
		printf("6 128MB memory spaces have been applied for!\n");
	else
		printf("Application for memory allocation failed!\n");
	getchar();
	
	// 释放第2、3、5号的128MB空间
	free(m2);
	printf("No. 2 memory space has been released!\n");
	getchar();
	free(m3);
	printf("No. 3 memory space has been released!\n");
	getchar();
	free(m5);
	printf("No. 5 memory space has been released!\n");
	getchar();
	
	// 再分配1024MB
	m7 = (int *) malloc(pow(2,28)*sizeof(int));
	if(m7 != NULL)
		printf("1024MB memory spaces have been applied for!\n");
	else
		printf("Application for memory allocation failed!\n");
	getchar();
	
	// 再分配64M内存
	m8 = (int *) malloc(pow(2,24)*sizeof(int));
	if(m8 != NULL)
		printf("64MB memory spaces have been applied for!\n");
	else
		printf("Application for memory allocation failed!\n");
	getchar();
	
	
	free(m1);free(m4);free(m6);free(m7);free(m8);
	return 0;
}
