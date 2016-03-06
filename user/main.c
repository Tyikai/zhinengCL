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
	AT24Cxx_Init();
	AT24Cxx_WriteOneByte(56,0x18);
	LCD_Init();
	usmart_dev.init(72);
	while(1)
	{
		UI_Run();
		delay_ms(500);
	}
}

