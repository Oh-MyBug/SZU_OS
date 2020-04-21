#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define LINE_SIZE 256
#define NUM_LINE 16

// 用于创建信号量时的识别id
const char * queue_mutex = "queue_mutex";
const char * queue_empty = "queue_empty";
const char * queue_full = "queue_full";

// 生产者消费者公用的缓冲区，含读写指针
// line_write和line_read，以及缓冲数据区
// buffer[NUM_LINE][LINE_SIZE],buffer[X]
// 指向第X行信息，buffer[X][Y]指向X行信息
// 的第Y个字符

struct shared_mem_st{
	char buffer[NUM_LINE][LINE_SIZE];	// 表示有NUM_LINE行LINE_SIZE列的缓冲数据区
	int line_write;	// 用于表示正在写第几行
	int line_read;	// 用于表示正在读第几行
};