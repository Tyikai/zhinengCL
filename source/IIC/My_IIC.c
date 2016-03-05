#include "My_IIC.h"
#include "delay.h"
/*.............................................................................
@Name		:	IIC_Init
@Param	:	void
@return	:	void
@Info		:	��ʼ��IIC
.............................................................................*/
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 		//PB6,PB7 �����
}
/*.............................................................................
@Name		:	IIC_Start
@Param	:	void
@return	:	void
@Info		:	����IIC��ʼ�ź�
.............................................................................*/
void IIC_Start(void)
{
	SDA_OUT();																//SDA�����
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 0;															//START��when CLK is hight, DATA change from
																						//high to low
	delay_us(4);
	IIC_SCL = 0;															//ǯסIIC���ߣ�׼�����ͻ��������
}
/*.............................................................................
@Name		:	IIC_Stop
@Param	:	void
@return	:	void
@Info		:	����IICֹͣ�ź�
.............................................................................*/
void IIC_Stop(void)
{
	SDA_OUT();																//sda�����
	IIC_SCL = 0;
	IIC_SDA = 0;															//STOP: when CLK is high, DATA change from
																						//low to high
	delay_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1;
	delay_us(4);															//����IIC���߽����ź�
}
/*.............................................................................
@Name		:	IIC_Wait_Ack
@Param	:	void
@return	:	u8 ����Ӧ���� 0 ����Ӧ��ɹ� 1 ����Ӧ��ʧ��
@Info		:	�ȴ�Ӧ����Ϣ����
.............................................................................*/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();																	//SDA����Ϊ����
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
	IIC_SCL = 0;															//ʱ�����0
	return 0;
}
/*.............................................................................
@Name		:	IIC_Ack
@Param	:	void
@return	:	void
@Info		:	����ACKӦ���ź�
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
@Info		:	����ACK ��Ӧ���ź�
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
@Param	:	u8 txd ����������
@return	:	void
@Info		:	ͨ��IIC����һ���ֽ�
.............................................................................*/
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0;															//����ʱ�ӿ�ʼ���ݴ���
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
@Param	:	u8 ack ack = 1ʱ ����ACK�ź� ACK = 0ʱ������nACK�ź�
@return	:	u8 ��ȡ��������
@Info		:	ͨ��IIC��ȡһ���ֽ�
.............................................................................*/
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	SDA_IN();																	//SDA����Ϊ����
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
		IIC_NotAck();														//����nACK�ź�
	}else
	{
		IIC_Ack();															//����ACK
	}
	return receive;
}
