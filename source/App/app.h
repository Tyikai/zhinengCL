#ifndef APP_H_
#define APP_H_
#include "sys.h"
void Start_Work(void);
void Start_Task(void *pdata);								//开始任务函数接口
void Led_Task(void *pdata);									//LED任务函数接口
#endif


