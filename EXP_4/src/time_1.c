#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
 
int main(){
	timeval starttime,endtime;
	int *m;
	int i, sum = 3*pow(2, 28);	// 3*pow(2, 28)*sizeof(int) = 3GB
	double runtime;
	
	printf("Process %d has been generated!\n", getpid());
	
	m = (int *)malloc(sum*sizeof(int));
	printf("3GB memory space has been applied for!\n");
	
	//相隔4KB进行读写就是相隔1024个int进行读写
	gettimeofday(&starttime,0);
	for(i=0;i<sum;i += 1024){
		m[i]++;
	}
	gettimeofday(&endtime,0);
	
	runtime = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
	runtime /=2000;//ms
	printf("%lfms = %lfs", runtime, runtime/1000);
	
	free(m);
	return 0;
}