#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <semaphore.h>

#define MAXREADER 1
#define NUM 30

/************************************************************
 *	// 目录项结构
 *  一个目录项包含了文件名和文件类型，						*
 *  当文件为目录时，起始盘块指示了目录表所在的盘块号；		*
 *  当文件为文件时，起始盘块指示了FCB所在的盘块号。			*
 ************************************************************/
struct dirUnit{
    char fileName[NUM]; //文件名
    char type;          //文件类型,0为目录,1为文件
    int startBlock;     //FCB起始盘块
};

/*************************************************************
 *	//目录表结构：
 *  本系统规定一个目录表只占用一个盘块，一个目录项大小为36B，*
 *  所以一个目录表中最多可含28个目录项，dirUnitAmount记录每个*
 *  目录表中已含有的目录项数目。系统在初始化时，会自动生成一 *
 *  个空的根目录表存放于磁盘中，作为用户的初始位置，用户所有 *
 *  的目录和文件都以这个表为根进行树状目录结构的展开。		 *
 *  当创建一个目录表时，系统会自动为目录表加上一项名为”..”的 *
 *  目录项，指示父目录表的位置。							 *
 *************************************************************/
#define DIRTABLE_MAX_SIZE 28    //目录表项最大值
struct dirTable {
    int dirUnitAmount;//目录项数目
    struct dirUnit dirs[DIRTABLE_MAX_SIZE];//目录项列表
};

/***************************************************************
 *	//文件控制块结构：
 *  文件控制块包含了文件数据的起始位置和大小。dataSize,readptr *
 *  是为文件的读写操作而准备的，记录文件已写入的内容长度（不可 *
 *  超过文件大小），和当前读取的位置。Link记录了文件的链接数， *
 *  用于文件的共享，当文件的链接数为0时，系统可以回收文件的空  *
 *  间。同样的，一个FCB大小为20B，但也用一个盘块保存。		   *
 *  由于采用的是连续分配方式，所以系统规定文件被创建时，必须给 *
 *  出文件的大小，而且后期也不能修改文件的大小。			   *
 ***************************************************************/
struct FCB {
    int blockNum;   //文件数据起始盘块号
    int fileSize;   //文件大小，盘块为单位
    int dataSize;   //已写入的内容大小，字节为单位
    int readptr;    //读指针，字节为单位
    int link;       //文件链接数
	
	int readCount;	//读进程计数
};

void initRootDir();	// 初始化根目录
char* getPath();	// 获得绝对路径					
void showDir();		// 展示当前目录 ls
int changeDir(char dirName[]);	// 切换目录 cd
int changeName(char oldName[], char newName[]);	// 修改文件名或者目录名 mv
int creatFile(char fileName[], int fileSize);	// 创建文件 open
int creatDir(char dirName[]);					// 创建目录 mkdir
// 创建FCB
int creatFCB(int fcbBlockNum, int fileBlockNum, int fileSize);			
// 添加目录项					
int addDirUnit(struct dirTable* myDirTable, char fileName[], int type, int FCBBlockNum);	
int deleteFile(char fileName[]);									// 删除文件 rm
int releaseFile(int FCBBlock, int unitIndex);						// 释放文件内存
int deleteDirUnit(struct dirTable* myDirTable, int unitIndex);		// 删除目录项
int deleteDir(char dirName[]);										// 删除目录 rmdir
int deleteFileInTable(struct dirTable* myDirTable, int unitIndex);	// 删除文件/目录项
int my_read(char fileName[], int length);							// 读文件
int my_write(char fileName[], char content[]);						// 写文件
int findUnitInTable(struct dirTable* myDirTable, char unitName[]);	// 从目录中查找目录项目
