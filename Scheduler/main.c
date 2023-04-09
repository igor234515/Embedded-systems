#include "stm32f746xx.h"
#include <stdint.h>

void SysCall(void);

#define STACK_SIZE 256

typedef struct {
  unsigned int sp;
  unsigned int stack[STACK_SIZE];
  int priority;
} tast_t;

volatile tast_t *sched_current_task;
volatile tast_t *sched_next_task;
volatile tast_t *running_task;

volatile tast_t taskA;
volatile tast_t taskB;
volatile tast_t taskC;

int cntA;
int cntB;
int ntD;

int worktimeA;
int worktimeB;
int worktimeC;
volatile int sleeptimeA;
volatile int sleeptimeB;
volatile int sleeptimeC;
int timeA;
int timeB;
int timeC;

volatile extern int syscnt;
void TaskA(void)
{
  while(1) {
	  timeA = syscnt;
	HAL_Delay(50);
    cntA++;
    sleeptimeB += syscnt - timeA;
    sleeptimeC += syscnt - timeA;
    worktimeA += syscnt - timeA;
    SysCall();
  }
}

void TaskB(void)
{
  while(1) {
	timeB = syscnt;
	HAL_Delay(50);
    cntB++;
    sleeptimeA += syscnt - timeB;
    sleeptimeC += syscnt - timeB;
    worktimeB += syscnt - timeB;
    SysCall();
  }
}

void TaskD(void)
{
  while(1) {
	 timeC = syscnt;
    ntD++;
    HAL_Delay(50);
    sleeptimeB += syscnt - timeC;
    sleeptimeA += syscnt - timeC;
    worktimeC += syscnt - timeC;
    SysCall();
  }
}

int Substractpriority(tast_t structure)
{
	structure.priority -=1;
	if (structure.priority < 0) structure.priority = 0;
	return structure.priority;
}

int Addpriority(tast_t structure)
{
	structure.priority +=1;
	if (structure.priority > 2) structure.priority = 3;
	return structure.priority;
}

void ChangeRunningTask(void)
{
	running_task = sched_next_task;
	if (worktimeA > 100) {taskA.priority = Substractpriority(taskA); worktimeA = 0;}
	if (worktimeB > 100) {taskB.priority =  Substractpriority(taskB); worktimeB = 0;}
	if (worktimeC > 100) {taskC.priority = Substractpriority(taskC); worktimeC = 0;}
	if (sleeptimeA > 200) {taskA.priority = Addpriority(taskA); sleeptimeA = 0;}
	if (sleeptimeB > 200) {taskB.priority = Addpriority(taskB); sleeptimeB = 0;}
	if (sleeptimeC > 200) {taskC.priority = Addpriority(taskC); sleeptimeC = 0;}
	if ((taskA.priority >= taskB.priority)&& (taskA.priority >= taskC.priority))
		  sched_next_task = &taskA;
	if ((taskB.priority >= taskA.priority)&& (taskB.priority >= taskC.priority))
	  	  sched_next_task = &taskB;
	if ((taskC.priority >= taskB.priority)&& (taskC.priority >= taskA.priority))
	  	  sched_next_task = &taskC;


//  running_task = sched_next_task;
//  // task scheduling: round-robin for 2 tasks
//  tast_t *tmp = sched_current_task;
//  sched_current_task = sched_next_task;
//  sched_next_task = tmp;
}

int main(void)
{
  SysTick_Config(SystemCoreClock/1000);
  taskA.stack[STACK_SIZE-1] = 0x01000000UL; // xPSR
  taskB.stack[STACK_SIZE-1] = 0x01000000UL; // xPSR
  taskC.stack[STACK_SIZE-1] = 0x01000000UL;

  taskA.stack[STACK_SIZE-2] = (unsigned int)TaskA | 1; // PC for return from interrupt
  taskB.stack[STACK_SIZE-2] = (unsigned int)TaskB | 1; // PC for return from interrupt
  taskC.stack[STACK_SIZE-2] = (unsigned int)TaskD | 1;

  // ################## Fill registers with stub values only for debug #####################
  //taskA.stack[STACK_SIZE-3] = LR; // old saved LR
  taskA.stack[STACK_SIZE-4] = 12; // saved R12
  taskA.stack[STACK_SIZE-5] = 3; // saved R3
  taskA.stack[STACK_SIZE-6] = 2; // saved R2
  taskA.stack[STACK_SIZE-7] = 1; // saved R1
  taskA.stack[STACK_SIZE-8] = 0; // saved R0
  taskA.stack[STACK_SIZE-9] = 11; // saved R11
  taskA.stack[STACK_SIZE-10] = 10; // saved R10
  taskA.stack[STACK_SIZE-11] = 9; // saved R9
  taskA.stack[STACK_SIZE-12] = 8; // saved R8
  taskA.stack[STACK_SIZE-13] = 7; // saved R7
  taskA.stack[STACK_SIZE-14] = 6; // saved R6
  taskA.stack[STACK_SIZE-15] = 5; // saved R5
  taskA.stack[STACK_SIZE-16] = 4; // saved R4

  //taskB.stack[STACK_SIZE-3] = LR; // old saved LR
  taskB.stack[STACK_SIZE-4] = 12; // saved R12
  taskB.stack[STACK_SIZE-5] = 3; // saved R3
  taskB.stack[STACK_SIZE-6] = 2; // saved R2
  taskB.stack[STACK_SIZE-7] = 1; // saved R1
  taskB.stack[STACK_SIZE-8] = 0; // saved R0
  taskB.stack[STACK_SIZE-9] = 11; // saved R11
  taskB.stack[STACK_SIZE-10] = 10; // saved R10
  taskB.stack[STACK_SIZE-11] = 9; // saved R9
  taskB.stack[STACK_SIZE-12] = 8; // saved R8
  taskB.stack[STACK_SIZE-13] = 7; // saved R7
  taskB.stack[STACK_SIZE-14] = 6; // saved R6
  taskB.stack[STACK_SIZE-15] = 5; // saved R5
  taskB.stack[STACK_SIZE-16] = 4; // saved R4
  // ######################################################################################

  taskA.sp = &taskA.stack[STACK_SIZE-16];
  taskB.sp = &taskB.stack[STACK_SIZE-16];
  taskC.sp = &taskC.stack[STACK_SIZE-16];
  taskA.priority = 2;
  taskB.priority = 1;
  taskC.priority = 0;
  sched_next_task = &taskB;
  sched_current_task = &taskA;
  running_task = sched_current_task;

  __NVIC_EnableIRQ(SVCall_IRQn);
  /* Switch to unprivileged mode with PSP stack */
  __set_PSP((uint32_t)(&(taskA.stack[STACK_SIZE-16])));
  __set_CONTROL(0x03);

  TaskA();

  while(1);
}
