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
int main(void)
{
	/*
	Init();								//硬件初始化初始化
	OSInit();							//系统初始化
	Start_Work();					//启动系统
	OSStart();						//系统运行*/
	u8 t=0;
	delay_init();	
	uart_init(9600);					//初始化串口
	Led_Init();
	LCD_Init();
	//TP_Init();
	usmart_dev.init(72);
	UI_Run();
	while(RTC_Init())
	{
		printf("初始化时钟出错\n");
		delay_ms(500);
	}
	while(1)
	{
		LED = !LED;
		RTC_Show(0,0);
		delay_ms(500);
	}
}

