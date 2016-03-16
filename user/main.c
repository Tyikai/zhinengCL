#include "Init.h"
#include "app.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "24Cxx.h"
#include "lcd.h"
#include "touch.h"
#include "rtc.h"
#include "showEN.h"
#include "usmart.h"
#include "ui.h"
#include "led.h"
#include "beep.h"
#include "showCH.h"
#include "rtc.h"
#include "bh1750.h"
#include "limit.h"
#include "tb6612.h"
int main(void)
{
	/*
	Init();								//Ӳ����ʼ����ʼ��
	OSInit();							//ϵͳ��ʼ��
	Start_Work();					//����ϵͳ
	OSStart();						//ϵͳ����*/
	u8 t=0;
	delay_init();	
	uart_init(9600);					//��ʼ������
	Led_Init();
	LCD_Init();
	Limit_Init();
	//TP_Init();
	usmart_dev.init(72);
	UI_Run();
	//TB6612_Init();
	tp_dev.init();
	while(RTC_Init())
	{
		printf("��ʼ��ʱ�ӳ���\n");
		
		delay_ms(500);
	}
	/*
	while(BH1750_Init())
	{
		printf("��ʼ����ǿ����������\n");
		delay_ms(500);		
	}*/
	while(1)
	{
		LED = !LED;
		RTC_Show(0,0);
		delay_ms(500);
		
	}
}

