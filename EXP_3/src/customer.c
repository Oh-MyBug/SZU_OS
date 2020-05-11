#include <stdio.h>    
#include <sys/ipc.h>    
#include <sys/shm.h>    
#include <sys/stat.h>
#include <semaphore.h>    
#include <fcntl.h>    
#include <stdlib.h>    
#include <unistd.h>    
#include <string.h>

#include "shm_com_sem.h"


int main(void)
{
	void *shared_memory = (void *)0;	// 共享内存(缓冲区指针)，分配的共享内存的原始首地址
	struct shared_mem_st *shared_stuff;	// 将无类型共享存储区转换为shared_mem_st类型的指针，将指向shared_memory
	
	int shmid;	// 共享内存的id
	int num_read;
	pid_t fork_result;	// 进程复制结果
	sem_t *sem_queue, *sem_queue_empty, *sem_queue_full;	
	// 访问共享内存的互斥量、空缓冲区、满缓冲区信号量。皆为信号量指针
	
	shmid = shmget((key_t)1234,sizeof(struct shared_mem_st), 0666|IPC_CREAT);	// 获取共享内存区，并挂入内存
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
	
	// 将缓冲区指针转化为shared_mem_st类型
	shared_stuff = (struct shared_mem_st *) shared_memory;
	
	// 获取producer创建的三个信号量，根据名字"queue_mutex""queue_empty"和"queue_full"识别
	sem_queue = sem_open("queue_mutex",0);  //sem=sem_open(sem_name,O_CREAT,0644,1);
    sem_queue_empty = sem_open("queue_empty",0);  //赋值为NUM_LINE,即buffer可用行数
    sem_queue_full = sem_open("queue_full",0);  //赋值为0  	
	
	// 父进程创建了一个子进程
	fork_result = fork();
	if(fork_result == -1){
		fprintf(stderr, "Fork failure\n");
	}
	if(fork_result==0){//子进程
		while(1){
			// 等待queue_full满缓冲区信号量以及queue_mutex互斥信号量
			sem_wait(sem_queue_full);
			sem_wait(sem_queue);
			// 加入适当的时延以便于观察
			sleep(3);
			// 如果生产者停止生产（quit），消费者停止消费
			if(strcmp(shared_stuff->buffer[shared_stuff->line_read],"quit") == 0){
				// 释放queue_mutex互斥信号量以及queue_full满缓冲区信号量
				sem_post(sem_queue);
				sem_post(sem_queue_full);	//需要通知另一个消费者生产者已停止生产
				// 打印停止消费信息
				printf("Producer no longer produces!--from child to father\n");
				break;
			}
			else{
				// 打印消费信息
				printf("===========customer(child):%d============\n",getpid());
				printf("current product No.%d\n",shared_stuff->line_read);
				printf("current product：%s\n",shared_stuff->buffer[shared_stuff->line_read]);
				shared_stuff->line_read = (shared_stuff->line_read+1)%NUM_LINE; 
				// 释放queue_mutex互斥信号量以及queue_full满缓冲区信号量
				sem_post(sem_queue);
				sem_post(sem_queue_empty);
			}
		}
		
		sem_unlink(queue_mutex);  
        sem_unlink(queue_empty);  
        sem_unlink(queue_full);
	}
	else{//父进程
		while(1){
			// 等待queue_full满缓冲区信号量以及queue_mutex互斥信号量
			sem_wait(sem_queue_full);
			sem_wait(sem_queue);
			// 加入适当的时延以便于观察
			sleep(3);
			// 如果生产者停止生产（quit），消费者停止消费
			if(strcmp(shared_stuff->buffer[shared_stuff->line_read],"quit") == 0){
				// 释放queue_mutex互斥信号量以及queue_full满缓冲区信号量
				sem_post(sem_queue);
				sem_post(sem_queue_full);	//需要通知另一个消费者生产者已停止生产
				// 打印停止消费信息
				printf("Producer no longer produces!--from father to child\n");
				break;
			}
			else{
				// 打印消费信息
				printf("===========customer(father):%d============\n",getpid());
				printf("current product No.%d\n",shared_stuff->line_read);
				printf("current product：%s\n",shared_stuff->buffer[shared_stuff->line_read]);
				shared_stuff->line_read = (shared_stuff->line_read+1)%NUM_LINE; 
				// 释放queue_mutex互斥信号量以及queue_full满缓冲区信号量
				sem_post(sem_queue);
				sem_post(sem_queue_empty);
			}
		}
		
		sem_unlink(queue_mutex);  
        sem_unlink(queue_empty);  
        sem_unlink(queue_full);
		// 等待子进程结束进程
		waitpid(fork_result,NULL,0);  
	}
	exit(EXIT_SUCCESS);

}
