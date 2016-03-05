#include "Init.h"
#include "app.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "24Cxx.h"
#include "lcd.h"
#include "touch.h"
int main(void)
{
	/*
	Init();								//Ӳ����ʼ����ʼ��
	OSInit();							//ϵͳ��ʼ��
	Start_Work();					//����ϵͳ
	OSStart();						//ϵͳ����*/
	delay_init();	
	uart_init(9600);					//��ʼ������
	AT24Cxx_Init();
	AT24Cxx_WriteOneByte(56,0x18);
	LCD_Init();
	while(1)
	{
	
	}

}

