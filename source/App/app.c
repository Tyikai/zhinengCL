#include "app.h"
#include "sys.h"
#include "ucos_ii.h"
#include "led.h"
#include "delay.h"
/*.............................................................................
																	开始任务
.............................................................................*/																	
#define START_TASK_PRIO			10							//设置任务优先级
#define START_STK_SIZE			64							//设置堆栈大小
OS_STK	START_TASK_STK[START_STK_SIZE];			//创建任务空间
/*.............................................................................
																	LED任务
.............................................................................*/		
#define LED_TASK_PRIO				15							//设置任务优先级
#define LED_STK_SIZE				32							//设置堆栈大小
OS_STK LED_TASK_STK[LED_STK_SIZE];					//创建任务空间
/*.............................................................................
@Name		:	Start_Task
@Param	:	void *pdata 函数指针
@return	:	void
@Info		:	开始任务，用于进入临界区创建任务
.............................................................................*/
void Start_Task(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_ENTER_CRITICAL();											//进入临界区
	OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
																						//创建LED任务
	OSTaskSuspend(START_TASK_PRIO);						//挂起开始任务
	OS_EXIT_CRITICAL();												//退出临界区
}
/*.............................................................................
@Name		:	Start_Work
@Param	:	void 
@return	:	void
@Info		:	系统启动任务
.............................................................................*/
void Start_Work(void)
{
	OSTaskCreate(Start_Task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
																						//创建开始任务
}
/*.............................................................................
@Name		:	Led_Task
@Param	:	void *pdata 函数指针
@return	:	void
@Info		:	LED任务，系统的运行指示灯
.............................................................................*/
void Led_Task(void *pdata)
{
	while(1)
	{
		LED = 0;
		delay_ms(200);
		LED = 1;
		delay_ms(300);
		printf("来自灯\n");
	}
}



