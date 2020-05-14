#include <stdio.h> 
#include <unistd.h> 

int main(int argc, char ** argv ) {
	int pid = fork();
	if(pid == -1 ) {
		printf("error!\n");
	} else if( pid ==0 ) {
		printf("This is the child process!\n");
		char *argv[]={"pstree", "2536", NULL, NULL};
		char *envp[]={"PATH=/usr/bin", NULL};                 
		execve("/usr/bin/pstree", argv, envp);
		printf("this printf()will not be exec,because it will be replaced by /usr/bin/pstree code!\n");
	} else {
		printf("This is the parent process! child process id = %d\n", pid);
		getchar();         
	}         
	return 0; 
} 
