#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
using namespace std;
 
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
	timeval starttime,endtime;
	double runtime;
	pthread_t id[16];
	int i,ret;
	int pc[16];
	int x = sum /16;
	for(i=-1;++i<16;){
		pc[i] = i * x;
	}
	
	printf("Process %d has been generated!\n", getpid());
	
	gettimeofday(&starttime,0);
	for(i=-1;++i<16;){ 
		ret=pthread_create(&id[i],NULL,visit,&pc[i]);
	}
	while(a!=16){}
	gettimeofday(&endtime,0);
	
	runtime = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
	runtime /=1000;//ms
	printf("%lfms = %lfs", runtime, runtime/1000);
	delete[] p;
	return 0;
}