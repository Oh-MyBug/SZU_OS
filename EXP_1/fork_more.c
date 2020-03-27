#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
int tprintf (const char*fmt, ...);
int main(void){
	int i = 0, j = 0;
	const int CREATE_NUM = 5;
	pid_t pid;
	printf("Hello from Parent Process, PID is %d.\n", getpid());
	for (i = 0; i < CREATE_NUM; i ++){
		pid = fork();
		if (pid > 0)
			tprintf("Parent forked one child process--%d.\n", pid);
		if (pid == 0)
			break;
		if (pid < 0) 
			tprintf("Failed to create child process!\n");
	}
	if (i < CREATE_NUM) { // child process
		sleep(1);
		printf("Hello from Child Process No.%d --pid = %d.\n", i+1, getpid());
	}
	if (i == CREATE_NUM) { // parent process
		pid_t wpid;
		while ((wpid = waitpid(-1, NULL, 0)) != -1){
			tprintf("Child Process(pid = %d) has exited.\n", wpid);
		}
		tprintf("Parent had exited.\n");
	}
	return 0;
}       
int tprintf (const char*fmt, ...){
	va_list args;
	struct tm *tstruct;
	time_t tsec;
	tsec = time(NULL);
	tstruct = localtime(&tsec);
	printf("%02d:%02d:%02d: %5d|", tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, getpid());
	va_start(args, fmt);
	return vprintf(fmt, args);
}	
