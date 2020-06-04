#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
 
int sum = 3*pow(2, 28);	// 3*pow(2, 28)*sizeof(int) = 3GB
int *m = (int *)malloc(sum*sizeof(int));
int a = 0;
void* visit(void* arg){
	int i = *(int *)arg;
	int end = i + sum/16;
	for(;i < end;i += 1024){
		++m[i];
	}
	++a;
}

int main(){
	clock_t start,end;
	double runtime;
	pthread_t id[16];
	int i,ret;
	int pc[16];
	int x = sum /16;
	for(i=-1;++i<16;){
		pc[i] = i * x;
	}
	
	printf("Process %d has been generated!\n", getpid());
	
	start = clock();
	for(i=-1;++i<16;){ 
		ret=pthread_create(&id[i],NULL,visit,&pc[i]);
	}
	while(a!=16){}
	end = clock();
	
	runtime = (double)(finish - start) / CLOCKS_PER_SEC;    
	printf( "%f seconds\n", duration ); 
	
	free(m);
	return 0;
}