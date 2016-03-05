#ifndef MY_IIC_H_
#define MY_IIC_H_
#include "sys.h"
#define SDA_IN() 	{GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=8<<28;}				//PB7 浮空输入
#define SDA_OUT() 	{GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=3<<28;}			//PB7 推挽输出
#define IIC_SCL 		PBout(6)								//PB6		SCL
#define IIC_SDA			PBout(7)								//PB7  	SDA
#define READ_SDA 		PBin(7)									//输入SDA
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NotAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(u8 ack);
#endif

