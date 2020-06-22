#include <stdio.h>    
#include <sys/ipc.h>    
#include <sys/shm.h>    
#include <fcntl.h>    
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>    
#include <unistd.h>    
#include <string.h>  
#include "shm_com_sem.h"


int main(void)
{
	void *shared_memory = (void *)0;	// 共享内存(缓冲区指针)，分配的共享内存的原始首地址   
	struct shared_mem_st *shared_stuff; // 将无类型共享存储区转换为shared_mem_st类型的指针，将指向shared_memory
	char key_line[256];
	
	int shmid;	//共享内存的id
	
	sem_t *sem_queue, *sem_queue_empty, *sem_queue_full;
	// 访问共享内存的互斥量、空缓冲区、满缓冲区信号量。皆为信号量指针
	
	shmid = shmget((key_t)1234, sizeof(struct shared_mem_st), 0666|IPC_CREAT);	// 获取共享内存区，并挂入内存
	if(shmid == -1){
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	printf("shimd:%d\n", shmid);
	shared_memory = shmat(shmid, 0, 0);	// 创建共享存储段之后，将进程连接到它的地址空间
    if(shared_memory == (void*)-1)   {  
        fprintf(stderr, "shmat failed\n"); 
        exit(EXIT_FAILURE);
    }  	
	
	shared_stuff = (struct shared_mem_st*)shared_memory;	// 将缓冲区指针转换为share_mem_st类型
	
	// 下面创建三个信号量	
	// queue_mutex互斥信号量，初始化为1
	sem_queue = sem_open("queue_mutex",O_CREAT,0644,1);
	// queue_empty空缓冲区信号量，初始化为NUM_LINE，即buffer可用行数
    sem_queue_empty = sem_open("queue_empty",O_CREAT,0644,NUM_LINE);  
	// queue_full满缓冲区信号量，初始化为0
    sem_queue_full = sem_open("queue_full",O_CREAT,0644,0); 

	// 读写指针初始化，开始时都指向第0行
	shared_stuff->line_write = 0;
	shared_stuff->line_read = 0;
	
	// 不断从控制台读入按键输入的字符行
	// Read and put input into buffer
	while(1){
		// 提示可以输入，并用gets()读入按键行到key_line中
		printf("Produce your product('quit' for exit):");
		gets(key_line);
		
		// 将输入的行写入缓冲区，要有信号量操作
		// 等待queue_empty空缓冲区信号量以及queue_mutex互斥信号量
		sem_wait(sem_queue_empty);
		sem_wait(sem_queue);
		//将key_line写入共享内存	
		strncpy(shared_stuff->buffer[shared_stuff->line_write],key_line,LINE_SIZE);
		// 打印当前生产者产生的商品
		printf("============================\n");
		printf("current product No.%d\n",shared_stuff->line_write);
		printf("current product: %s\n",shared_stuff->buffer[shared_stuff->line_write]);
		// 写完指向下一行，当写最后一行时，指向第一行
		shared_stuff->line_write = (shared_stuff->line_write+1)%NUM_LINE;  
		// 释放queue_mutex互斥信号量以及queue_full满缓冲区信号量
		sem_post(sem_queue);
		sem_post(sem_queue_full);
		
		// 如果键入"quit"则退出
		if(strcmp(key_line, "quit") == 0){
			break;
		}
	}
	
	// 因键入"quit"从前面while()循环中跳出到此处，程序退出前，释放信号量
	sem_unlink(queue_mutex);
	sem_unlink(queue_empty);  
	sem_unlink(queue_full);
	if(shmdt(shared_memory)==-1){	//把共享内存从当前进程中分离 
		fprintf(stderr,"shmdt failed\n");  
		exit(EXIT_FAILURE);
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1){	//删除共享内存     
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);
    }
	printf("Producer no longer produce products!");
}
