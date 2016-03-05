#include "rtc.h"
#include "sys.h"
#include "delay.h"
_calendar_obj calendar;
/*.............................................................................
							十二个月，每个月拥有的天数
.............................................................................*/	
const u8 mon_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 
/*.............................................................................
							星期 ,月修正数据表
.............................................................................*/
const u8 table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};	
/*.............................................................................
@Name	:	RTC_NVIC_Config
@Param	:	void
@return	:	void
@Info	:	RTC的NVIC配置
.............................................................................*/
static void RTC_NVIC_Config(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
											//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
											//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
											//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);			//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}
/*.............................................................................
@Name	:	RTC_Init
@Param	:	void
@return	:	void
@Info	:	初始化RTC时钟,同时检测时钟是否工作正常
.............................................................................*/
u8 RTC_Init(void)
{
											//检查是不是第一次配置时钟
	u8 temp=0;
 
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x3050)		
											//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	 			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	
											//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);		//使能后备寄存器访问 
		BKP_DeInit();						//复位备份区域 	
		RCC_LSEConfig(RCC_LSE_ON);			//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	
											//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
		if(temp>=250)return 1;				//初始化时钟失败,晶振有问题	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);				//能RTC时钟  
		RTC_WaitForLastTask();				//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();				//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();				//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();				/// 允许配置	
		RTC_SetPrescaler(32767); 			//设置RTC预分频的值
		RTC_WaitForLastTask();				//等待最近一次对RTC寄存器的写操作完成
		RTC_Set(2016,3,1,12,0,0);  			//设置时间	
		RTC_ExitConfigMode(); 				//退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X3050);//向指定的后备寄存器中写入用户程序数据
	}
	else									//系统继续计时
	{
		RTC_WaitForSynchro();				//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();				//等待最近一次对RTC寄存器的写操作完成
	}
	RTC_NVIC_Config();						//RCT中断分组设置		    				     
	RTC_Get();								//更新时间	
	return 0; 								//ok
}
/*.............................................................................
@Name	:	RTC_IRQHandler
@Param	:	void
@return	:	void
@Info	:	RTC时钟中断，一秒钟一次
.............................................................................*/
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{							
		RTC_Get();							//更新时间   
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)	//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	//清闹钟中断	  	   
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}
/*.............................................................................
@Name	:	RTC_Set
@Param	: 	u16 syear 年份
			u8 smon 月份
			u8 sday 日期
			u8 hour 小时
			u8 min  分钟
			u8 sec  秒钟
@return	:	u8 设置结果 0成功， 1失败
@Info	:	设置时钟，自定义
.............................................................................*/
u8  RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
	u16 t;
	u32 seccount = 0;
	if(syear <1970 || syear > 2099)
	{
		return 1;
	}
	for(t=1970;t<syear;t++)
	{
		if(Is_Leap_Year(t))
		{
			seccount += 31622400;			//闰年的秒钟数
		}else
		{
			seccount += 31536000;			//平年的秒钟数
		}
	}
	smon -= 1;								//从0开始编号
	for(t=0;t<smon;t++)
	{
		seccount += (u32) mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear) && t == 1)
		{
			seccount += 86400;				//润年，二月份多一天
		}
	}
	seccount += (u32) (sday-1)*86400;		//日期拥有的秒数
	seccount += (u32) hour*3600;			//小时有有的秒数
	seccount += (u32) min*60;				//分钟拥有的秒数
	seccount += sec;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	
											//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);			//使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);				//设置RTC计数器的值

	RTC_WaitForLastTask();					//等待最近一次对RTC寄存器的写操作完成  	
	return 0;	 	
}