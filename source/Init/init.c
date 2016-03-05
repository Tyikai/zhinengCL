#include "init.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "usart.h"
/*.............................................................................
@Name		:	Init()
@Param	:	void
@return	:	void
@Info		:	��ʼ��ϵͳ������Ӳ��
.............................................................................*/
void Init(void)
{
	delay_init();															//��ʼ����ʱģ��
	uart_init(9600);					//��ʼ������
	Led_Init();																//��ʼ��LEDģ��
	//LCD_Init();																//LCDģ��
}


