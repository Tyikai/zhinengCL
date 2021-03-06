#include "24Cxx.h"
#include "My_IIC.h"
#include "delay.h"
/*.............................................................................
@Name	:	AT24Cxx_Init
@Param	:	void 
@return	:	void
@Info	:	初始化 EEPROM芯片 AT24C02
.............................................................................*/
void AT24Cxx_Init(void)
{
	IIC_Init();
}
/*.............................................................................
@Name	:	AT24Cxx_ReadOneByte
@Param	:	u16 ReadAddr 开始读取的地址 
@return	:	u8 读取到的数据
@Info	:	在AT24Cxx指定地址读取一个数据
.............................................................................*/
u8 AT24Cxx_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);				//发送写指令,前4bit固定为1010 紧接3bit为 A2 A1 A0 硬件决定,
											//(系统最多可接8个设备2^3=8) 最后1bit决定读写 1 读 0 写
											//0xA0 = 1010 000 0写
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);			//发送高地址
	}else
	{
		IIC_Send_Byte(0xA0+((ReadAddr/256)<<1));//获取地址高8位
		                                    //C01、C02 1010 A2 A1 A0 R/W
		                                    //C04      1010 A2 A1 a8 R/W ----a8地址的第9位
											//C08			1010 A2 a9 a8 R/W	----a9地址的第10位
											//C16      1010 a10 a9 a8 R/W --- a10地址的第11位
											//当IIC设备为C16时近可以挂载1个，因为无地址可修改
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr%256); 			//发送地址低8位
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0xA1);					//进入接收模式
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();								//产生一个停止条件
	return temp;
}
/*.............................................................................
@Name	:	AT24Cxx_WriteOneByte
@Param	:	u16 WriteAddr 写入数据的目的地址
			8 data 待写入的数据
@return	:	void
@Info	:	在AT24Cxx指定的地址写入一个数据
.............................................................................*/
void AT24Cxx_WriteOneByte(u16 WriteAddr, u8 data)
{
	IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);				//发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);		//发送高地址
	}else
	{
		IIC_Send_Byte(0xA0+((WriteAddr/256)<<1));//发送器件高地址
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr%256);			//发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(data);					//发送字节
	IIC_Wait_Ack();
	IIC_Stop();								//产生一个停止条件
	delay_ms(10);
}
/*.............................................................................
@Name	:	AT24Cxx_WriteLenByte
@Param	:	u16 WriteAddr 写入数据的目的地址
			8 data 待写入的数据
			u8 len 写入数据的长度
@return	:	void
@Info	:	在AT24Cxx里面指定的地址开始写入长度为len的数据
.............................................................................*/
void AT24Cxx_WriteLenByte(u16 WriteAddr, u32 data, u8 len)
{
	u8 t;
	for(t=0;t<len;t++)
	{
		AT24Cxx_WriteOneByte(WriteAddr++, (data>>(8*t))&0xFF);
	}
}
/*.............................................................................
@Name	:	AT24Cxx_ReadLenByte
@Param	:	u16 WriteAddr 写入数据的目的地址
			u8 len 数据长度 1,2,4
@return	:	u32 读取到的数据
@Info	:	从AT24Cxx芯片中读取指定长度的数据
.............................................................................*/
u32 AT24Cxx_ReadLenByte(u16 ReadAddr, u8 len)
{
	u8 t;
	u32 temp = 0;
	for(t=0;t<len;t++)
	{
		temp <<= 8;
		temp += AT24Cxx_ReadOneByte(ReadAddr+len-t-1);
	}
	return temp;
}
/*.............................................................................
@Name	:	AT24Cxx_Check
@Param	:	void
@return	:	u8 返回检查结果 0 成功  1 失败 
@Info	:	检查AT24Cxx是否成功
.............................................................................*/
u8 AT24Cxx_Check(void)
{
	u8 temp;
	temp = AT24Cxx_ReadOneByte(255);		//避免每次开机都写AT24Cxx
	if(temp == 0x55)
	{
		return 0;							//排除第一次初始化的情况
	}else
	{
		AT24Cxx_WriteOneByte(255, 0x55);
		temp = AT24Cxx_ReadOneByte(255);
		if(temp == 0x55)
		{
			return 0;
		}
	}
	return 1;
}
/*.............................................................................
@Name	:	AT24Cxx_Read
@Param	:	u16 ReadAddr 指定读取的地址
			u8 *pBuffer 存储数据数组的首地址
			u16 num 读取数据的个数
@return	:	void
@Info	:	从AT24Cxx指定的地址读取指定个数的数据
.............................................................................*/
void AT24Cxx_Read(u16 ReadAddr, u8 *pBuffer, u16 num)
{
	while(num --)
	{
		*pBuffer ++ =AT24Cxx_ReadOneByte(ReadAddr ++);
	}
}
/*.............................................................................
@Name	:	AT24Cxx_Write
@Param	:	u16 WriteAddr 写入数据的地址
			u8 *pBuffer写入数据的数组首地址
			u16 num 写入数据的长�
@return	:	void
@Info	:	在AT24Cxx中指定的位置写入指定长度的数据
.............................................................................*/
void AT24Cxx_Write(u16 WriteAddr, u8 *pBuffer, u16 num)
{
	while(num--)
	{
		AT24Cxx_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr ++;
		pBuffer ++;
	}
}

