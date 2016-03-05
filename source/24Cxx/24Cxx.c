#include "24Cxx.h"
#include "My_IIC.h"
#include "delay.h"
/*.............................................................................
@Name		:	AT24Cxx_Init
@Param	:	void 
@return	:	void
@Info		:	��ʼ�� EEPROMоƬ AT24C02
.............................................................................*/
void AT24Cxx_Init(void)
{
	IIC_Init();
}
/*.............................................................................
@Name		:	AT24Cxx_ReadOneByte
@Param	:	u16 ReadAddr ��ʼ��ȡ�ĵ�ַ 
@return	:	 u8 ��ȡ��������
@Info		:	��AT24Cxxָ����ַ��ȡһ������
.............................................................................*/
u8 AT24Cxx_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);										//����дָ��,ǰ4bit�̶�Ϊ1010 ����3bitΪ A2 A1 A0 Ӳ������,
																						//(ϵͳ���ɽ�8���豸2^3=8) ���1bit������д 1 �� 0 д
																						//0xA0 = 1010 000 0д
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);							//���͸ߵ�ַ
	}else
	{
		IIC_Send_Byte(0xA0+((ReadAddr/256)<<1));//��ȡ��ַ��8λ
		                                        //C01��C02 1010 A2 A1 A0 R/W
		                                        //C04      1010 A2 A1 a8 R/W ----a8��ַ�ĵ�9λ
																						//C08			1010 A2 a9 a8 R/W	----a9��ַ�ĵ�10λ
																						//C16      1010 a10 a9 a8 R/W --- a10��ַ�ĵ�11λ
																						//��IIC�豸ΪC16ʱ�����Թ���1������Ϊ�޵�ַ���޸�
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr%256); 							//���͵�ַ��8λ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0xA1);											//�������ģʽ
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();																//����һ��ֹͣ����
	return temp;
}
/*.............................................................................
@Name		:	AT24Cxx_WriteOneByte
@Param	:	u16 WriteAddr д�����ݵ�Ŀ�ĵ�ַ
					8 data ��д�������
@return	:	void
@Info		:	��AT24Cxxָ���ĵ�ַд��һ������
.............................................................................*/
void AT24Cxx_WriteOneByte(u16 WriteAddr, u8 data)
{
	IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);										//����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);						//���͸ߵ�ַ
	}else
	{
		IIC_Send_Byte(0xA0+((WriteAddr/256)<<1));//���������ߵ�ַ
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr%256);							//���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(data);											//�����ֽ�
	IIC_Wait_Ack();
	IIC_Stop();																//����һ��ֹͣ����
	delay_ms(10);
}
/*.............................................................................
@Name		:	AT24Cxx_WriteLenByte
@Param	:	u16 WriteAddr д�����ݵ�Ŀ�ĵ�ַ
					8 data ��д�������
					u8 len д�����ݵĳ���
@return	:	void
@Info		:	��AT24Cxx����ָ���ĵ�ַ��ʼд�볤��Ϊlen������
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
@Name		:	AT24Cxx_ReadLenByte
@Param	:	u16 WriteAddr д�����ݵ�Ŀ�ĵ�ַ
					u8 len ���ݳ��� 1,2,4
@return	:	u32 ��ȡ��������
@Info		:	��AT24CxxоƬ�ж�ȡָ�����ȵ�����
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
@Name		:	AT24Cxx_Check
@Param	:	void
@return	:	u8 ���ؼ���� 0 �ɹ�  1 ʧ�� 
@Info		:	���AT24Cxx�Ƿ�ɹ�
.............................................................................*/
u8 AT24Cxx_Check(void)
{
	u8 temp;
	temp = AT24Cxx_ReadOneByte(255);					//����ÿ�ο�����дAT24Cxx
	if(temp == 0x55)
	{
		return 0;																//�ų���һ�γ�ʼ�������
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
@Name		:	AT24Cxx_Read
@Param	:	u16 ReadAddr ָ����ȡ�ĵ�ַ
					u8 *pBuffer �洢����������׵�ַ
					u16 num ��ȡ���ݵĸ���
@return	:	void
@Info		:	��AT24Cxxָ���ĵ�ַ��ȡָ������������
.............................................................................*/
void AT24Cxx_Read(u16 ReadAddr, u8 *pBuffer, u16 num)
{
	while(num --)
	{
		*pBuffer ++ =AT24Cxx_ReadOneByte(ReadAddr ++);
	}
}
/*.............................................................................
@Name		:	AT24Cxx_Write
@Param	:	u16 WriteAddr д�����ݵĵ�ַ
					u8 *pBufferд�����ݵ������׵�ַ
					u16 num д�����ݵĳ��
@return	:	void
@Info		:	��AT24Cxx��ָ����λ��д��ָ�����ȵ�����
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

