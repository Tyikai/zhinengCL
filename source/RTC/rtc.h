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
#endif

