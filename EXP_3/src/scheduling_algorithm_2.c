// 模拟实现各种CPU调度算法 
#include "stdio.h"
#include <stdlib.h>
#include <time.h>

#define getpch(type) (type*)malloc(sizeof(type))
typedef struct pcb PCB;
struct pcb {				// 定义进程控制块PCB 
	int id;					// 标识符 
	char name[10];			// 名称 
	int time_start;			// 到达时间 
	int time_need;			// 服务时间 
	int time_left;			// 剩余运行时间 
	int time_used;			// 已使用的CPU时间 
// 在时间片机制中，time_need = time_left + time_used 
	char state;				// 进程状态 
}; 

//*********************** 系统函数 ***********************
void __sleep(int n){			// 停顿几秒 
	clock_t goal;
	goal = (clock_t)n * CLOCKS_PER_SEC + clock();
	while(goal > clock());
}

char __keygo(){				// 程序暂停，按任意键继续 
	char c;
	printf("Press any key to continue.....\n");
	c = getchar();
	return c;
}

//*********************** 用户函数 ***********************

//---------------------- 数据设计区域 ----------------------
int time_unit = 2;			// 时间片长度
#define maxnum 10			// 最大进程数量
int num = 5;				// 实际进程数量
PCB pcbdata[maxnum] = {
	{1000, "A", 0, 4, 4, 0, 'R'},
	{1001, "B", 1, 3, 3, 0, 'R'},
	{1002, "C", 2, 5, 5, 0, 'R'},
	{1003, "D", 3, 2, 2, 0, 'R'},
	{1004, "E", 4, 4, 4, 0, 'R'},
}; 

int ready[maxnum];			// 就绪队列，存放进程在pcbdata中的位置
// 例如上面的数据经过某种调度算法排序为CBADE
// 就绪队列第一个进程为C
// 存放在ready数组的第一个数值为2，因为pcbdata[2]是C进程的PCB 
// 就绪队列第二个进程为B
// 存放在ready数组的第一个数值为1，因为pcbdata[1]是B进程的PCB 
int order[maxnum];			// 记录排序使用哪个数值作为排序对象

void input(){				// 手工输入数据 
	int i;
	printf("The total number of processes is ");
	scanf("%d",&num);
	for(i = 0; i < num; i ++){
		pcbdata[i].id = 1000 + i;											// 系统自动分配标识符 
		printf("Please enter the %dth process name: ", i+1);
		scanf("%s", pcbdata[i].name);
		printf("Please enter the %dth process arrival time: ", i+1);
		scanf("%d", &pcbdata[i].time_start);
		printf("Please enter the %dth process service time: ", i+1);
		scanf("%d", &pcbdata[i].time_need);
		// 剩余运行时间，初始值和服务时间相同 
		pcbdata[i].time_left = pcbdata[i].time_need;
		printf("\n");
		pcbdata[i].time_used = 0;											// 已使用CPU时间初始值为0 
		pcbdata[i].state = 'R';												// 进程初始状态为就绪R 
	}
}

//---------------------- 调度函数 ----------------------
void FCFS(){									// 先来先服务算法 
// 已到达时间为排序对象，从小到大排列，静态排序 
	int i, j, temp;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_start;		// 到达时间time_start作为排序对象 
		ready[i] = i;							// 对应order数组中排序对象在pcbdata中的位置
	}
	for(i = 0; i < num; i ++){					// 冒泡排序 
		for(j = i+1; j < num; j ++){
			if(order[i] > order[j]){
				temp = order[i];
				order[i] = order[j];
				order[j] = temp;
				temp = ready[i];
				ready[i] = ready[j];
				ready[j] = temp;
			}
		}
	}
	printf("==========FCFS: No preemption, no time slice==========\n");
	temp = pcbdata[ready[0]].time_start;
	for(i = 0; i < num; i ++){
		printf("The %dth process--%s, ", i+1, pcbdata[ready[i]].name);
		printf("arrival time--%d, service time--%d\n", 
				pcbdata[ready[i]].time_start, 
				pcbdata[ready[i]].time_need);
		printf("This process is running...\n");
		__sleep(1);
		printf("This process has completed running.\n");
		temp += pcbdata[ready[i]].time_need;				// 进程完成时间 
		j = temp - pcbdata[ready[i]].time_start;			// 周转时间 
		k = (float)j/pcbdata[ready[i]].time_need;			// 带权周转时间 
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%.2f\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
} 

void SJF(){
// 以服务时间为排序对象，从小到大排序，可能有多次排序
	int run_clock = 0;						// 模拟CPU运行时钟 
	int i, j;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_need;	// 以服务时间为排序对象，以当前已到达的进程为基准暂时存储 
	}
	for(i = 0; i < num; i ++){
		int min_time_need = 9999;
		int index = 0;
		// 循环遍历查找排序对象，该对象需同时满足：1. run_clock已超过该对象到达时间 2. 服务时间最短 
		for(j = 0; j < num; j ++){
			if(order[j] < min_time_need && pcbdata[j].time_start <= run_clock){
				min_time_need = order[j];
				index = j;
			}
		}
		ready[i] = index;			// 将查找到的排序对象存入就绪队列 
		run_clock += order[index];	// 同时更新CPU运行时钟 
		order[index] = 9999;		// 将已存入就绪队列的排序对象数值设为最大，避免重复查找该对象 
	}
	printf("==========SJF: No preemption, no time slice==========\n");
	int temp = pcbdata[ready[0]].time_start;
	for(i = 0; i < num; i ++){
		printf("The %dth process--%s, ", i+1, pcbdata[ready[i]].name);
		printf("arrival time--%d, service time--%d\n", 
				pcbdata[ready[i]].time_start, 
				pcbdata[ready[i]].time_need);
		printf("This process is running...\n");
		__sleep(1);
		printf("This process has completed running.\n");
		temp += pcbdata[ready[i]].time_need;				// 进程完成时间 
		j = temp - pcbdata[ready[i]].time_start;			// 周转时间 
		k = (float)j/pcbdata[ready[i]].time_need;			// 带权周转时间 
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%.2f\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
}

void HRRN(){
// 以响应比为排序对象，从大到小排序，可能有多次排序 
	int run_clock = 0;												// 模拟CPU运行时钟 
	int waited[num];
	int i,j;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_need;							// order数组存储运行时间 
		waited[i] = run_clock - pcbdata[i].time_start;				// waited数组存储已等待时间 
	}
	for(i = 0; i < num; i ++){
		double max_rp = 0;
		int index = 0;
		for(j = 0; j < num; j ++){
			double rp = 1 + (1.0*waited[j]/order[j]);				// 计算当前进程的响应比 
			// 循环遍历查找排序对象，该对象需同时满足：1. run_clock已超过该对象到达时间 2. 响应比最大 
			if(rp > max_rp && pcbdata[j].time_start <= run_clock){ 
				max_rp = rp;
				index = j;
			}
		}
		ready[i] = index;			// 将查找到的排序对象存入就绪队列 
		run_clock += order[index];	// 同时更新CPU运行时钟 
		// 更新所有进程的等待时间 
		for(j = 0; j < num; j ++){
			if(waited[j] == -9999 || j == index){
				waited[j] = -9999;
			} else {
				waited[j] = run_clock - pcbdata[j].time_start;
			}
		}
	}
	printf("==========HRRN: No preemption, no time slice==========\n");
	int temp = pcbdata[ready[0]].time_start;
	for(i = 0; i < num; i ++){
		printf("The %dth process--%s, ", i+1, pcbdata[ready[i]].name);
		printf("arrival time--%d, service time--%d\n", 
				pcbdata[ready[i]].time_start, 
				pcbdata[ready[i]].time_need);
		printf("This process is running...\n");
		__sleep(1);
		printf("This process has completed running.\n");
		temp += pcbdata[ready[i]].time_need;				// 进程完成时间 
		j = temp - pcbdata[ready[i]].time_start;			// 周转时间 
		k = (float)j/pcbdata[ready[i]].time_need;			// 带权周转时间 
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%.2f\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
}

void RR(){
// 按照先来先服务并使用时间片轮转 
}

void RRMF(){
// 多级反馈调度队列，抢占式调度 
printf("对不起宝贝，未能实现...");
}

void main(){
	int i = 0, sch = 99;
	while(sch != 0){
		printf("\nPlease select one of the scheduling algorithms:\n");
		printf("(1) FCFS--First Come First Service\n");
		printf("(2) SJF--Shortest Job First\n");
		printf("(3) HRRN--Highest Response Ratio Next\n");
		printf("(4) RR--Round Robin\n");
		printf("(5) RRMF--Round Robin with Multiple Feedback\n");
		printf("(0) Exit\n");
		printf("Please enter a number above: ");
		scanf("%d",&sch);
		switch(sch){
			case 1: FCFS();break;
			case 2: input();SJF();break;
			case 3: input();HRRN();break;
			case 4: RR();break;
			case 5: RRMF();break;
			case 0: printf("exit\n");break;
		}
	}
	__keygo();
}
