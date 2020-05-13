#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node{
	int id;
	char name[10];
	int time;
	int time_start;
	int time_need;
	int time_left;
	int time_used;
	char state;
	struct node *next;
}PCB;

PCB *back_queue = NULL, *ready_queue = NULL, *run_queue = NULL, *finish_queue = NULL;

void _sleep(int n){
	clock_t goal;
	goal = (clock_t)n*CLOCKS_PER_SEC + clock();
	while(goal > clock());
}

char _keygo(){
	char c;
	printf("Press any key to continue.....\n");
	c = getchar();
	return c;
}

int time_unit = 2;
int run_clock = 0;
int number = 1;
#define MAX_PROCESS_NUM 10
int num = 5;
PCB pcbdata[MAX_PROCESS_NUM] = {
	{1000, "A", 4, 0, 4, 4, 0, 'N', NULL},
	{1001, "B", 3, 1, 3, 3, 0, 'N', NULL},
	{1002, "C", 5, 2, 5, 5, 0, 'N', NULL},
	{1003, "D", 2, 3, 2, 2, 0, 'N', NULL},
	{1004, "E", 4, 4, 4, 4, 0, 'N', NULL}
};

int ready[MAX_PROCESS_NUM];
int order[MAX_PROCESS_NUM];
void input(){
	int i;
	printf("The total number of processes is ");
	scanf("%d",&num);
	printf("Time-slicing size: ");
	scanf("%d", &time_unit);
	for(i = 0; i < num; i ++){
		pcbdata[i].id = 1000 + i;
		printf("Please enter the %dth process name: ", i+1);
		scanf("%s", pcbdata[i].name);
		printf("Please enter the %dth process arrival time: ", i+1);
		scanf("%d", &pcbdata[i].time_start);
		printf("Please enter the %dth process service time: ", i+1);
		scanf("%d", &pcbdata[i].time_need);
		pcbdata[i].time_left = pcbdata[i].time_need;
		printf("\n");
		pcbdata[i].time_used = 0;
		pcbdata[i].state = 'R';
	}
}

void GetFirstReady();
void GetFirstBack();
void Output();
void InsertBack(PCB *in);
void InsertReady(PCB *in);
void InsertFinish(PCB *in);
void TimeCreate();
void RoundRun();
void print(PCB *in);

void GetFirstReady(){
	run_queue = ready_queue;
	if(ready_queue != NULL){
		run_queue->state = 'R';
		ready_queue = ready_queue->next;
		run_queue->next = NULL;
	}
}

void GetFirstBack(){
	if(back_queue!=NULL){
		PCB *tmp = (PCB *) malloc(sizeof(PCB));
		strcpy(tmp->name,back_queue->name);
		tmp->time_start = back_queue->time_start;
		tmp->time_left = back_queue->time_left;
		tmp->time_used = back_queue->time_used;
		tmp->state = back_queue->state;
		tmp->time = back_queue->time;
		InsertReady(tmp);
		back_queue = back_queue->next;
	}
}

void Output(){
	number = 1;
	PCB *p;
	printf("Round %d, CPU time: %d\n", number++, run_clock);
	printf("Process Name\t CPU Time Used\t Remaining Running Time\t Process State\n");
	p = ready_queue;
	while(p != NULL){
		printf("%s\t\t %d\t\t %d\t\t %c\t\n", p->name, p->time_used, p->time_left, p->state);
		p = p->next;
	}
	p = finish_queue;
	while(p != NULL){	
		printf("%s\t\t %d\t\t %d\t\t %c\t\n", p->name, p->time_used, p->time_left, p->state);
		p = p->next;
	}
	p = run_queue;
	while(p != NULL){
		printf("%s\t\t %d\t\t %d\t\t %c\t\n", p->name, p->time_used, p->time_left, p->state);
		p = p->next;
	}
	printf("\n");
}

void InsertBack(PCB *in){
	in->state = 'N';
	PCB *fst;
	fst = back_queue;
	if(back_queue == NULL){
		in->next = back_queue;
		back_queue = in;
	}
	else{
		while(fst->next != NULL){
			fst = fst->next;
		}
		in->next = fst->next;
		fst->next = in;
	}
}

void InsertReady(PCB *in){
	in->state = 'W';
	PCB *fst;
	fst = ready_queue;
	if(ready_queue == NULL){
		in->next = ready_queue;
		ready_queue = in;
	}else {
		while(fst->next != NULL){
			fst = fst->next;
		}
		in->next = fst->next;
		fst->next = in;
	}
}

void InsertFinish(PCB *in){
	in->state = 'F';
	PCB *fst;
	fst = finish_queue;
	if(fst == NULL){
		in->next = finish_queue;
		finish_queue = in;
	}else {
		while(fst->next != NULL){
			fst = fst->next;
		}
		in->next = fst->next;
		fst->next = in;
	}
}

void print(PCB *in){
	PCB *temp = in;
	while(temp){
		printf("%s", temp->name);
		temp = temp->next;
	}
}

void TimeCreate(){
	input();
	PCB *tmp;
	int i;
	for(i = 0; i < num; i ++){
		if((tmp = (PCB *)malloc(sizeof(PCB))) == NULL){
			perror("malloc");
			exit(1);
		}
		int min_start_time = 9999;
		int index = 0;
		int j;
		for(j = 0; j < num; j ++){
			if(pcbdata[j].time_start < min_start_time){
				min_start_time = pcbdata[j].time_start;
				index = j;
			}
		}
		strcpy(tmp->name, pcbdata[index].name);
		tmp->time_start = pcbdata[index].time_start;
		tmp->time_left = pcbdata[index].time_left;
		pcbdata[index].time_start = 9999;
		tmp->time_used = 0;
		tmp->state = 'N';
		tmp->time = 0;
		InsertBack(tmp);
	}
}

void RoundRun(){
	run_clock = 0;
	int flag = 1;
	GetFirstBack();
	printf("ready_queue: ");
	print(ready_queue);
	printf("\n");
	GetFirstReady();
	while(run_queue != NULL){
		Output();
		while(flag){
			run_clock ++;
			run_queue->time_used ++;
			run_queue->time_left --;
			run_queue->time ++;
			while(back_queue != NULL && back_queue->time_start <= run_clock){
				GetFirstBack();
			}
			if(run_queue->time_left == 0){
				InsertFinish(run_queue);
				flag = 0;
			} else if(time_unit == run_queue->time){
				run_queue->time = 0;
				InsertReady(run_queue);
				flag = 0;
			}
		}
		flag = 1;
		printf("ready queue: ");
		print(ready_queue);
		printf("\n");
		GetFirstReady();
	}
}

void FCFS(){
	int i, j, temp;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_start;
		ready[i] = i;
	}
	for(i = 0; i < num; i ++){
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
		printf("arrival time--%d, service time--%d\n", pcbdata[ready[i]].time_start, pcbdata[ready[i]].time_need);
		printf("This process is running...\n");
		_sleep(1);
		printf("This process has completed running.\n");
		temp += pcbdata[ready[i]].time_need;
		j = temp - pcbdata[ready[i]].time_start;
		k = (float)j/pcbdata[ready[i]].time_need;
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%.1f\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
}

void SJF(){
	int run_clock = 0;
	int i, j;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_need;
	}
	for(i = 0; i < num; i ++){
		int min_time_need = 9999;
		int index = 0;
		for(j = 0; j < num; j ++){
			if(order[j] < min_time_need && pcbdata[j].time_start <= run_clock){
				min_time_need = order[j];
				index = j;
			}
		}
		ready[i] = index;
		run_clock += order[index];
		order[index] = 9999;
	}
	printf("==========SJF==========\n");
	int temp = pcbdata[ready[0]].time_start;
	for(i = 0; i < num; i ++){
		printf("The %dth process--%s, ", i+1, pcbdata[ready[i]].name);
		printf("arrival time--%d, service time--%d\n", pcbdata[ready[i]].time_start, pcbdata[ready[i]].time_need);
		printf("This process is running...\n");
		_sleep(1);
		printf("This process has completed running.\n");
		temp += pcbdata[ready[i]].time_need;
		j = temp - pcbdata[ready[i]].time_start;
		k = (float)j/pcbdata[ready[i]].time_need;
		printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%lf\n", temp, j, k);
	}
	printf("---------All processes are scheduled!---------\n");
}

void HRRN(){
	run_clock = 0;
	int *waited = (int *)calloc(num, sizeof(int));
	int i,j;
	double k;
	for(i = 0; i < num; i ++){
		order[i] = pcbdata[i].time_need;
		waited[i] = run_clock - pcbdata[i].time_start;
	}
	for(i = 0; i < num; i ++){
		double max_rp = 0;
		int index = 0;
		for(j = 0; j < num; j ++){
			double rp = 1 + (1.0*waited[j]/order[j]);
			if(rp > max_rp && pcbdata[j].time_start <= run_clock){
				max_rp = rp;;
				index = j;
			}
		}
		ready[i] = index;
		run_clock += order[index];
		for(j = 0; j < num; j ++){
			if(waited[j] == 9999 || j == index){
				waited[j] = -9999;
			} else {
				waited[j] = run_clock - pcbdata[j].time_start;
			}
		}
		printf("=========HRF=========\n");
		int temp = pcbdata[ready[0]].time_start;
		for(i = 0; i < num; i ++){
			printf("The %dth process--%s, ", i+1, pcbdata[ready[i]].name);
			printf("arrival time--%d, service time--%d\n", pcbdata[ready[i]].time_start, pcbdata[ready[i]].time_need);
			printf("This process is running...\n");
			_sleep(1);
			printf("This process has completed running.\n");
			temp += pcbdata[ready[i]].time_need;
			j = temp - pcbdata[ready[i]].time_start;
			k = (float)j/pcbdata[ready[i]].time_need;
			printf("Finish time--%d, Turnaround time--%d, Turnaround time with weighted--%lf\n", temp, j, k);
		}
		printf("---------All processes are scheduled!---------\n");		
	}
}

void RR(){
	TimeCreate();
	RoundRun();
	Output();
}

void RRMF(){
	printf("Unrealized...\n");
}

int main(){
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
			case 2: SJF();break;
			case 3: HRRN();break;
			case 4: RR();break;
			case 5: RRMF();break;
			case 0: printf("exit\n");break;
		}
	}
}
