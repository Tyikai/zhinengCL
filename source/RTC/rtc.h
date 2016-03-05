#ifndef RTC_H_
#define RTC_H_
#include "sys.h"
typedef struct{			//时间结构体
	u8 hour;
	u8 main;
	u8 sec;
	u16 w_year;
	u8 w_month;
	u8 w_day;
	u8 w_week;
}_calendar_obj;
extern _calendar_obj calendar;
static void RTC_NVIC_Config(void);
u8 RTC_Init(void);
void RTC_IRQHandler(void);
u8  RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec);
u8 Is_Leap_Year(u16 year);
u8 RTC_Get(void);
u8 RTC_Get_Week(u16 year, u8 month, u8 day);
#endif

