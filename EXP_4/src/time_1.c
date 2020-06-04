#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
 
int main(){
	clock_t start, finish;
	int *m;
	int i, sum = 3*pow(2, 28);	// 3*pow(2, 28)*sizeof(int) = 3GB
	double runtime;
	
	printf("Process %d has been generated!\n", getpid());
	
	m = (int *)malloc(sum*sizeof(int));
	printf("3GB memory space has been applied for!\n");
	
	//相隔4KB进行读写就是相隔1024个int进行读写
	start = clock();
	for(i=0;i<sum;i += 1024){
		m[i]++;
	}
	finish = clock();
	
	runtime = (double)(finish - start) / CLOCKS_PER_SEC;    
	printf( "%f seconds\n", runtime); 
	
	getchar();
	free(m);
	return 0;
}
