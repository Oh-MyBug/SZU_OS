// ģ��ʵ�ָ���CPU�����㷨 
#include "stdio.h"
#include <stdlib.h>
#include <time.h>

#define getpch(type) (type*)malloc(sizeof(type))
typedef struct pcb PCB;
struct pcb {				// ������̿��ƿ�PCB 
	int id;					// ��ʶ�� 
	char name[10];			// ���� 
	int time_start;			// ����ʱ�� 
	int time_need;			// ����ʱ�� 
	int time_left;			// ʣ������ʱ�� 
	int time_used;			// ��ʹ�õ�CPUʱ�� 
// ��ʱ��Ƭ�����У�time_need = time_left + time_used 
	char state;				// ����״̬ 
}; 

//*********************** ϵͳ���� ***********************
void __sleep(int n){			// ͣ�ټ��� 
	clock_t goal;
	goal = (clock_t)n * CLOCKS_PER_SEC + clock();
	while(goal > clock());
}

char __keygo(){				// ������ͣ������������� 
	char c;
	printf("Press any key to continue.....\n");
	c = getchar();
	return c;
}

//*********************** �û����� ***********************

//---------------------- ����������� ----------------------
int time_unit = 2;			// ʱ��Ƭ����
#define maxnum 10			// ����������
int num = 5;				// ʵ�ʽ�������
PCB pcbdata[maxnum] = {
	{1000, "A", 0, 4, 4, 0, 'R'},
	{1001, "B", 1, 3, 3, 0, 'R'},
	{1002, "C", 2, 5, 5, 0, 'R'},
	{1003, "D", 3, 2, 2, 0, 'R'},
	{1004, "E", 4, 4, 4, 0, 'R'},
}; 

int ready[maxnum];			// �������У���Ž�����pcbdata�е�λ��
// ������������ݾ���ĳ�ֵ����㷨����ΪCBADE
// �������е�һ������ΪC
// �����ready����ĵ�һ����ֵΪ2����Ϊpcbdata[2]��C���̵�PCB 
// �������еڶ�������ΪB
// �����ready����ĵ�һ����ֵΪ1����Ϊpcbdata[1]��B���̵�PCB 
int order[maxnum];			// ��¼����ʹ���ĸ���ֵ��Ϊ�������

void input(){				// �ֹ��������� 
	int i;
	printf("The total number of processes is ");
	scanf("%d",&num);
	for(i = 0; i < num; i ++){
		pcbdata[i].id = 1000 + i;											// ϵͳ�Զ������ʶ�� 
		printf("Please enter the %dth process name: ", i+1);
		scanf("%s", pcbdata[i].name);
		printf("Please enter the %dth process arrival time: ", i+1);
		scanf("%d", &pcbdata[i].time_start);
		printf("Please enter the %dth process service time: ", i+1);
		scanf("%d", &pcbdata[i].time_need);
		// ʣ������ʱ�䣬��ʼֵ�ͷ���ʱ����ͬ 
		pcbdata[i].time_left = pcbdata[i].time_need;
		printf("\n");
		pcbdata[i].time_used = 0;											// ��ʹ��CPUʱ���ʼֵΪ0 
		pcbdata[i].state = 'R';												// ���̳�ʼ״̬Ϊ����R 
	}
}

//---------------------- ���Ⱥ��� ----------------------
void FCFS(){									// �����ȷ����㷨 
// �ѵ���ʱ��Ϊ������󣬴�С�������У���̬���� 
	int i, j, temp;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_start;		// ����ʱ��time_start��Ϊ������� 
		ready[i] = i;							// ��Ӧorder���������������pcbdata�е�λ��
	}
	for(i = 0; i < num; i ++){					// ð������ 
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
		temp += pcbdata[ready[i]].time_need;				// �������ʱ�� 
		j = temp - pcbdata[ready[i]].time_start;			// ��תʱ�� 
		k = (float)j/pcbdata[ready[i]].time_need;			// ��Ȩ��תʱ�� 
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%.2f\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
} 

void SJF(){
// �Է���ʱ��Ϊ������󣬴�С�������򣬿����ж������
	int run_clock = 0;						// ģ��CPU����ʱ�� 
	int i, j;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_need;	// �Է���ʱ��Ϊ��������Ե�ǰ�ѵ���Ľ���Ϊ��׼��ʱ�洢 
	}
	for(i = 0; i < num; i ++){
		int min_time_need = 9999;
		int index = 0;
		// ѭ����������������󣬸ö�����ͬʱ���㣺1. run_clock�ѳ����ö��󵽴�ʱ�� 2. ����ʱ����� 
		for(j = 0; j < num; j ++){
			if(order[j] < min_time_need && pcbdata[j].time_start <= run_clock){
				min_time_need = order[j];
				index = j;
			}
		}
		ready[i] = index;			// �����ҵ�������������������� 
		run_clock += order[index];	// ͬʱ����CPU����ʱ�� 
		order[index] = 9999;		// ���Ѵ���������е����������ֵ��Ϊ��󣬱����ظ����Ҹö��� 
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
		temp += pcbdata[ready[i]].time_need;				// �������ʱ�� 
		j = temp - pcbdata[ready[i]].time_start;			// ��תʱ�� 
		k = (float)j/pcbdata[ready[i]].time_need;			// ��Ȩ��תʱ�� 
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%.2f\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
}

void HRRN(){
// ����Ӧ��Ϊ������󣬴Ӵ�С���򣬿����ж������ 
	int run_clock = 0;												// ģ��CPU����ʱ�� 
	int waited[num];
	int i,j;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_need;							// order����洢����ʱ�� 
		waited[i] = run_clock - pcbdata[i].time_start;				// waited����洢�ѵȴ�ʱ�� 
	}
	for(i = 0; i < num; i ++){
		double max_rp = 0;
		int index = 0;
		for(j = 0; j < num; j ++){
			double rp = 1 + (1.0*waited[j]/order[j]);				// ���㵱ǰ���̵���Ӧ�� 
			// ѭ����������������󣬸ö�����ͬʱ���㣺1. run_clock�ѳ����ö��󵽴�ʱ�� 2. ��Ӧ����� 
			if(rp > max_rp && pcbdata[j].time_start <= run_clock){ 
				max_rp = rp;
				index = j;
			}
		}
		ready[i] = index;			// �����ҵ�������������������� 
		run_clock += order[index];	// ͬʱ����CPU����ʱ�� 
		// �������н��̵ĵȴ�ʱ�� 
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
		temp += pcbdata[ready[i]].time_need;				// �������ʱ�� 
		j = temp - pcbdata[ready[i]].time_start;			// ��תʱ�� 
		k = (float)j/pcbdata[ready[i]].time_need;			// ��Ȩ��תʱ�� 
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%.2f\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
}

void RR(){
// ���������ȷ���ʹ��ʱ��Ƭ��ת 
}

void RRMF(){
// �༶�������ȶ��У���ռʽ���� 
printf("�Բ��𱦱���δ��ʵ��...");
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
