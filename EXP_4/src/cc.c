#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

int main(){
	int *m;
	int i, sum = 3*pow(2, 28);	// 3*pow(2, 28)*sizeof(int) = 3GB
	printf("Process %d has been generated!\n", getpid());
	getchar();
	
	m = (int *)malloc(sum*sizeof(int));
	printf("3GB memory space has been applied for!\n");
	
	// 相隔4KB进行读写就是像个1024个int进行读写
	printf("Enter to start writing!\n");
	getchar();
	
	for(i = 0; i < sum; i +=1024)
		m[i]++;
	printf("stop writing!\n");
	getchar();
	
	return 0;
}
