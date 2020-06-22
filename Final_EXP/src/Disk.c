#include "Disk.h"

#include <stdio.h>    
#include <sys/ipc.h>    
#include <sys/shm.h>    
#include <fcntl.h>    
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>    
#include <unistd.h>    
#include <string.h>  

struct shared_mem_st{
	char systemStartAddr[system_size * sizeof(char)];	// 系统起始地址
};

/* 创建一个100M的共享内存 */
// 共享内存(缓冲区指针)，分配的共享内存的原始首地址   
void *shared_memory = (void *)0;	
// 将无类型共享存储区转换为shared_mem_st类型的指针，将指向shared_memory
struct shared_mem_st *shared_stuff; 
// 共享内存的id
int shmid;	

//初始化系统
void initSystem()
{
	int i;	
	// 获取共享内存区，并挂入内存
	shmid = shmget((key_t)1234, sizeof(struct shared_mem_st), 0666|IPC_CREAT);	
	if(shmid == -1){
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	// 创建共享存储段之后，将进程连接到它的地址空间
	shared_memory = shmat(shmid, 0, 0);	
    if(shared_memory == (void*)-1)   {  
        fprintf(stderr, "shmat failed\n"); 
        exit(EXIT_FAILURE);
    }  	
	// 将缓冲区指针转换为share_mem_st类型
	shared_stuff = (struct shared_mem_st*)shared_memory;	
	
	//初始化盘块的位示图：当其值为“0”时，表示对应的盘块空闲；为“1”时，表示已经分配
    for(i = 0; i < block_count; i++)							// 系统盘块数目100*1024
        shared_stuff->systemStartAddr[i] = '0';
    //用于存放位示图的空间已被占用
    int bitMapSize = block_count * sizeof(char) / block_szie;	//位示图占用盘块数 = 盘块数/盘块大小 = 100
    for(i=0; i<bitMapSize; i++)									//从零开始分配
        shared_stuff->systemStartAddr[i] = '1';   				//盘块已被使用
}

//退出系统
void exitSystem()
{
    if(shmdt(shared_memory)==-1){	//把共享内存从当前进程中分离 
		fprintf(stderr,"shmdt failed\n");  
		exit(EXIT_FAILURE);
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1){	//删除共享内存     
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);
    }
}

//磁盘分配
int getBlock(int blockSize)
{
    int startBlock = 0;
    int sum = 0;
	int i,j;
    for(i = 0; i < block_count; i++)
    {
        if(shared_stuff->systemStartAddr[i] == '0')//可用盘块
        {
            if(sum == 0)//刚开始，设置开始盘块号
                startBlock = i;
            sum++;
            if(sum == blockSize)//连续盘块是否满足需求
            {
                //满足分配，置1
                for(j = startBlock; j < startBlock+blockSize; j++)
                    shared_stuff->systemStartAddr[j] = '1';
                return startBlock;
            }

        }
        else//已被使用,连续已经被打断
            sum = 0;
    }
    printf("not found such series memory or memory is full\n");
    return -1;
}
//获得盘块的物理地址
char* getBlockAddr(int blockNum)
{
    return shared_stuff->systemStartAddr + blockNum * block_szie; //偏移量单位为字节
}
//获得物理地址的盘块号
int getAddrBlock(char* addr)
{
    return (addr - shared_stuff->systemStartAddr)/block_szie;
}
//释放盘块
int releaseBlock(int blockNum, int blockSize)
{
	int i;
    int endBlock = blockNum + blockSize;
    //修改位示图盘块的位置为0
    for(i = blockNum; i < endBlock; i++)
        shared_stuff->systemStartAddr[i] = '0';
    return 0;
}
