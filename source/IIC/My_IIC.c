#include "My_IIC.h"
#include "delay.h"
/*.............................................................................
@Name		:	IIC_Init
@Param	:	void
@return	:	void
@Info		:	初始化IIC
.............................................................................*/
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 		//PB6,PB7 输出高
}
/*.............................................................................
@Name		:	IIC_Start
@Param	:	void
@return	:	void
@Info		:	产生IIC起始信号
.............................................................................*/
void IIC_Start(void)
{
	SDA_OUT();																//SDA线输出
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 0;															//START：when CLK is hight, DATA change from
																						//high to low
	delay_us(4);
	IIC_SCL = 0;															//钳住IIC总线，准备发送或接收数据
}
/*.............................................................................
@Name		:	IIC_Stop
@Param	:	void
@return	:	void
@Info		:	产生IIC停止信号
.............................................................................*/
void IIC_Stop(void)
{
	SDA_OUT();																//sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0;															//STOP: when CLK is high, DATA change from
																						//low to high
	delay_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1;
	delay_us(4);															//发送IIC总线结束信号
}
/*.............................................................................
@Name		:	IIC_Wait_Ack
@Param	:	void
@return	:	u8 返回应答结果 0 接收应答成功 1 接收应答失败
@Info		:	等待应答信息到来
.............................................................................*/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();																	//SDA设置为输入
	IIC_SDA = 1;
	delay_us(1);
	IIC_SCL = 1;
	delay_us(1);
	while(READ_SDA)
	{
		ucErrTime ++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;															//时钟输出0
	return 0;
}
/*.............................................................................
@Name		:	IIC_Ack
@Param	:	void
@return	:	void
@Info		:	产生ACK应答信号
.............................................................................*/
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}
/*.............................................................................
@Name		:	IIC_NotACK
@Param	:	void
@return	:	void
@Info		:	产生ACK 非应答信号
.............................................................................*/
void IIC_NotAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}
/*.............................................................................
@Name		:	IIC_Send_Byte
@Param	:	u8 txd 待发送数据
@return	:	void
@Info		:	通过IIC发送一个字节
.............................................................................*/
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0;															//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
		IIC_SDA = (txd&0x80)>>7;
		txd <<= 1;
		delay_us(2);
		IIC_SCL = 1;
		delay_us(2);
		IIC_SCL = 0;
		delay_us(2);
	}
}
/*.............................................................................
@Name		:	IIC_Read_Byte
@Param	:	u8 ack ack = 1时 发送ACK信号 ACK = 0时，发送nACK信号
@return	:	u8 读取到的数据
@Info		:	通过IIC读取一个字节
.............................................................................*/
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	SDA_IN();																	//SDA设置为输入
	for(i=0;i<8;i++)
	{
		IIC_SCL = 0;
		delay_us(2);
		IIC_SCL = 1;
		receive <<= 1;
		if(READ_SDA)
		{
			receive ++;
		}
		delay_us(1);
	}
	if(!ack)
	{
		IIC_NotAck();														//发送nACK信号
	}else
	{
		IIC_Ack();															//发送ACK
	}
	return receive;
}
