#ifndef os_time_H_
	#define os_time_H_
	#include"includes.h"
	void  OSTimeDly (INT32U ticks);
	INT8U  OSTimeDlyHMSM (INT8U   hours,INT8U   minutes,INT8U   seconds,INT16U  ms);
	INT8U  OSTimeDlyResume (INT8U prio);
	INT32U  OSTimeGet (void);
	void  OSTimeSet (INT32U ticks);
#endif
