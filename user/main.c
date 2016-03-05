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
	Init();								//硬件初始化初始化
	OSInit();							//系统初始化
	Start_Work();					//启动系统
	OSStart();						//系统运行*/
	delay_init();	
	uart_init(9600);					//初始化串口
	AT24Cxx_Init();
	AT24Cxx_WriteOneByte(56,0x18);
	LCD_Init();
	while(1)
	{
	
	}

}

