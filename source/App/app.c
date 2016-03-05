#include "app.h"
#include "sys.h"
#include "ucos_ii.h"
#include "led.h"
#include "delay.h"
/*.............................................................................
																	��ʼ����
.............................................................................*/																	
#define START_TASK_PRIO			10							//�����������ȼ�
#define START_STK_SIZE			64							//���ö�ջ��С
OS_STK	START_TASK_STK[START_STK_SIZE];			//��������ռ�
/*.............................................................................
																	LED����
.............................................................................*/		
#define LED_TASK_PRIO				15							//�����������ȼ�
#define LED_STK_SIZE				32							//���ö�ջ��С
OS_STK LED_TASK_STK[LED_STK_SIZE];					//��������ռ�
/*.............................................................................
@Name		:	Start_Task
@Param	:	void *pdata ����ָ��
@return	:	void
@Info		:	��ʼ�������ڽ����ٽ�����������
.............................................................................*/
void Start_Task(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_ENTER_CRITICAL();											//�����ٽ���
	OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
																						//����LED����
	OSTaskSuspend(START_TASK_PRIO);						//����ʼ����
	OS_EXIT_CRITICAL();												//�˳��ٽ���
}
/*.............................................................................
@Name		:	Start_Work
@Param	:	void 
@return	:	void
@Info		:	ϵͳ��������
.............................................................................*/
void Start_Work(void)
{
	OSTaskCreate(Start_Task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
																						//������ʼ����
}
/*.............................................................................
@Name		:	Led_Task
@Param	:	void *pdata ����ָ��
@return	:	void
@Info		:	LED����ϵͳ������ָʾ��
.............................................................................*/
void Led_Task(void *pdata)
{
	while(1)
	{
		LED = 0;
		delay_ms(200);
		LED = 1;
		delay_ms(300);
		printf("���Ե�\n");
	}
}



