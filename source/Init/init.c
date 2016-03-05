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
@Info		:	初始化系统所有软硬件
.............................................................................*/
void Init(void)
{
	delay_init();															//初始化延时模块
	uart_init(9600);					//初始化串口
	Led_Init();																//初始化LED模块
	//LCD_Init();																//LCD模块
}


