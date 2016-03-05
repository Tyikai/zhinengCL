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
	AT24Cxx_Init();
	AT24Cxx_WriteOneByte(56,0x18);
	LCD_Init();
	while(RTC_Init())
	{
		Show_String08_NotBK(0,0,"RTC ERROR");
		delay_ms(500);
		Show_String08_NotBK(0,0,"RTC Tring");
	}
	POINT_COLOR = BLUE;
	while(1)
	{
		if(t!=calendar.sec)
		{
			t = calendar.sec;
			Show_Num(0,16,calendar.w_year);
			Show_Num(0,32,calendar.w_month);
			Show_Num(0,48,calendar.w_day);
			Show_Num(0,64,calendar.hour);
			Show_Num(0,90,calendar.main);
			Show_Num(0,106,calendar.sec);
		}
		delay_ms(20);
	}

}

