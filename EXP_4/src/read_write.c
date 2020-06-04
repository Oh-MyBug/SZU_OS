#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

int main(){
	int *m;
	int i,sum = pow(2,26);
	printf("Process %d has been generated!\n", getpid());
	getchar();
	
	m = (int *)malloc(sum*sizeof(int)); // 256MB
	printf("256MB memory space has been applied for!\n");
	
	// 相隔4KB进行读写就是像个1024个int进行读写
	printf("Enter to start reading!\n");
	getchar();
	
	for(i = 0; i < sum; i +=1024)
		m[i];
	printf("stop reading!\n");
	getchar();
	
	printf("Enter to start writing!\n");
	getchar();
	
	for(i = 0; i < sum; i +=1024)
		m[i] = i;
	printf("stop writing!\n");
	getchar();
	
	return 0;
}
