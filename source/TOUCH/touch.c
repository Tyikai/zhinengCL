#include "touch.h"
#include "delay.h"
#include "lcd.h"
#include "24Cxx.h"
#include "ShowEN.h"
#include "ShowCH.h"
#include "ILI93xx.h"
#include "math.h"
#include "stdlib.h"
#include "key.h"
#include "key.h"
#define READ_TIMES		5											//��ȡ����
#define LOST_VAL			1											//��ʧֵ
#define ERR_RANGE			50										//��Χ
u8 CMD_RDX = 0xD0;													//��ȡX����ָ��
u8 CMD_RDY = 0x90;													//��ȡY����ָ��
_m_tp_dev tp_dev=
{
	TP_Init,																	//��ʼ��������������,����ָ��ָ�� u8 TP_Init(void)
	TP_Scan,																	//ɨ�败����, 0 ��Ļɨ�裬 1 �������� ,����ָ�� u8 TP_Scan(u8 tp)
	TP_Adjust,																// ������У׼, ����ָ�� u8 TP_Adjust(void)
	0,																				//ԭʼ����(��һ�ΰ���ʱ������)
	0,
	0,																				//�ʵ�״̬ [7] 0 �ɿ�  1 ���� [6] 0 �ް�������  1 �а�������
	0,																				//				������У׼����
	0,
	0,
	0,
	0,																				//������������																
};
extern _m_tp_dev tp_dev;
/*.............................................................................
@Name		:	TP_Write_Byte
@Param	:	u8 data		��д�������
@return	:	void
@Info		:	ͨ��(ģ��)SPI����������ICд��1byte����
.............................................................................*/
void TP_Write_Byte(u8 data)
{
	u8 t = 0;
	for(t=0;t<8;t++)
	{
		if(data & 0x80)
		{
			TDIN = 1;
		}else
		{
			TDIN = 0;
		}
		data <<= 1;
		TCLK = 0;
		TCLK = 1;																//��������Ч
	}
}
/*.............................................................................
@Name		:	TP_Read_AD(u8 cmd)
@Param	:	u8 cmd	ָ��
@return	:	u16 ��ȡ����adcֵ
@Info		:	ͨ��(ģ��)SPI��ȡ������������IC��adcֵ
.............................................................................*/
u16 TP_Read_AD(u8 cmd)
{
	u8 t=0;
	u16 data = 0;
	TCLK = 0;																	//������ʱ��
	TDIN = 0;																	//����������
	TCS = 0;																	//ѡ�д�����IC
	TP_Write_Byte(cmd);												//����������
	delay_us(6);															//ADS7856��ת��ʱ���Ϊ6��s
	TCLK = 0;
	delay_us(1);
	TCLK = 1;																	//��1��ʱ��,���BUSY
	TCLK = 0;
	for(t=0;t<16;t++)
	{
		data <<= 1;
		TCLK = 0;																//�½�����Ч
		TCLK = 1;
		if(DOUT)
		{
			data ++;
		}
	}
	data >>= 4;																//ֻ�и�12λ��Ч
	TCS = 1;																	//�ͷ�Ƭѡ
	return data;
}
/*.............................................................................
@Name		:	TP_Read_XOY
@Param	:	u8 cmdxy RDXָ���RDYָ��
@return	:	u16 ��ȡ��������ֵ
@Info		:	��ȡһ������ֵ X��Y
.............................................................................*/
u16 TP_Read_XOY(u8 cmdxy)
{
	u16 i, j;
	u16 buf[READ_TIMES];											//�����ȡ����ֵ
	u16 sum = 0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{
		buf[i] = TP_Read_AD(cmdxy);
	}
	for(i=0;i<READ_TIMES-1;i++)								//��������
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
	{
		sum += buf[i];
	}
	temp = sum / (READ_TIMES-2*LOST_VAL);
	return temp;
}
/*.............................................................................
@Name		:	TP_Read_XY
@Param	:	u16 *x	x����
					u16 *y  y����
@return	:	u8���ض�ȡ��� 0 ʧ��  1 �ɹ�
@Info		:	��ȡ(X,Y)����  ��ȡ��x��yֵС��50�ж�Ϊ��ȡʧ��
.............................................................................*/
u8 TP_Read_XY(u16 *x, u16 *y)
{
	u16 xtemp,ytemp;
	xtemp = TP_Read_XOY(CMD_RDX);							//��ȡx����
	ytemp = TP_Read_XOY(CMD_RDY);							//��ȡy����
	*x = xtemp;
	*y = ytemp;
	return 1;
}
/*.............................................................................
@Name		:	TP_Read_XY2
@Param	:	u16 *x	��ȡ����x����
					u16 *y	��ȡ����y����
@return	:	u8 ���ض�ȡ��� 0 ʧ��  1 �ɹ�
@Info		:	������ȡ2�δ�����������IC,�����ε�ƫ��ܳ���ERR_RANGE��Χ
					,����Ϊ������ȷ�������ȡʧ��
.............................................................................*/
u8 TP_Read_XY2(u16 *x, u16 *y)
{
	u16 x1,y1;
	u16 x2, y2;
	u8 flag;
	flag = TP_Read_XY(&x1, &y1);
	if(flag == 0)
	{
		return 0;
	}
	flag = TP_Read_XY(&x2, &y2);
	if(flag == 0)
	{
		return 0;
	}
	if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
		&&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
	{
		*x = (x1+x2)/2;
		*y = (y1+y2)/2;
		return 1;
	}else
	{
		return 0;
	}
}
/*.............................................................................
@Name		:	TP_Draw_Touch_Point
@Param	:	u16 x	x����
					u16 y y����
					u16 color ��ɫ
@return	:	void
@Info		:	��һ��������,����У׼
.............................................................................*/
void TP_Draw_Touch_Point(u16 x, u16 y, u16 color)
{
	POINT_COLOR = color;
	LCD_DrawLine(x-12, y, x+13, y);						// ����
	LCD_DrawLine(x, y-12, x, y+13);						// ����
	LCD_DrawPoint(x+1, y+1);
	LCD_DrawPoint(x-1, y+1);
	LCD_DrawPoint(x+1, y-1);
	LCD_DrawPoint(x-1, y-1);
	LCD_DrawCircle(x, y, 6);									//������Բ
}
/*.............................................................................
@Name		:	TP_Draw_Touch_Point
@Param	:	u16 x	x����
					u16 y y����
					u16 color ��ɫ
@return	:	void
@Info		:	��һ����ĵ� (2*2)
.............................................................................*/
void TP_Draw_Big_Point(u16 x, u16 y, u16 color)
{
	POINT_COLOR = color;
	LCD_DrawPoint(x, y);
	LCD_DrawPoint(x+1, y);
	LCD_DrawPoint(x, y+1);
	LCD_DrawPoint(x+1, y+1);
}
/*.............................................................................
@Name		:	TP_Scan
@Param	:	u8 tp 0 ��Ļ����  1 ��������(У׼�����ⳡ�ϲ���)
@return	:	u8 ������״̬ 0 �������޴���  1  �������д���
@Info		:	�л�������ģʽ
.............................................................................*/
u8 TP_Scan(u8 tp)
{
	if(PEN == 0)
	{
		if(tp)
		{
			TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]);
																						//��ȡ��������
		}else if(TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]))
																						//��ȡ��Ļ����
		{
			tp_dev.x[0] = tp_dev.xfac * tp_dev.x[0] + tp_dev.xoff;
																						//�����ת��Ϊ��Ļ����
			tp_dev.y[0] = tp_dev.yfac * tp_dev.y[0] + tp_dev.yoff;
		}
		if((tp_dev.sta & TP_PRES_DOWN) == 0)		//������������
		{
			tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES;
																						//��������
			tp_dev.x[4] = tp_dev.x[0];						//��¼��һ�ΰ���ʱ������
			tp_dev.y[4] = tp_dev.y[0];
		}
	}else
	{
		if(tp_dev.sta & TP_PRES_DOWN)						//������������
		{
			tp_dev.sta &= ~(1<<7);								//��ǰ����ɿ�
		}else
		{
			tp_dev.x[4] = 0;
			tp_dev.y[4] = 0;
			tp_dev.x[0] = 0xFFFF;
			tp_dev.y[0] = 0xFFFF;
		}
	}
	return (tp_dev.sta & TP_PRES_DOWN);				//���ص�ǰ�Ĵ�����״̬
}
#define SAVE_ADDR_BASE 16										//������EEPROM����ĵ�ַ�����ַ
/*.............................................................................
@Name		:	TP_Save_Adjdata
@Param	:	void
@return	:	void
@Info		:	����У׼����
.............................................................................*/
void TP_Save_Adjdata(void)
{
	s32 temp;																	//�з���32λ����
	temp = tp_dev.xfac * 100000000;						//����xУ������
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE, temp, 4);
	temp = tp_dev.yfac * 100000000;						//����yУ������
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE+4, temp, 4);
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE+8, tp_dev.xoff, 2);		//����xƫ����
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE+10, tp_dev.yoff, 2);	//����yƫ����
	AT24Cxx_WriteOneByte(SAVE_ADDR_BASE+12, tp_dev.touchtype);//������Ļ����
	temp = 0x0A;															//����Ѿ�У׼����
	AT24Cxx_WriteOneByte(SAVE_ADDR_BASE+13, temp);
}
/*.............................................................................
@Name		:	TP_Get_Adjdata
@Param	:	void
@return	:	u8 EEPROM�д��������
@Info		:	�õ�������EEPROM�е�У׼ֵ
.............................................................................*/
u8 TP_Get_Adjdata(void)
{
	s32 tempfac;
	tempfac = AT24Cxx_ReadOneByte(SAVE_ADDR_BASE+13);
	if(tempfac == 0x0A)												//�������Ѿ�У׼����
	{
		tempfac = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE, 4);
		tp_dev.xfac = (float) tempfac /100000000;//�õ�xУ׼����
		tempfac = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE+4, 4);
		tp_dev.yfac = (float) tempfac /100000000;//�õ�yУ׼����
		tp_dev.xoff = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE+8, 2);		//�õ�xƫ����
		tp_dev.yoff = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE+10, 2);	//�õ�yƫ����
		tp_dev.touchtype = AT24Cxx_ReadOneByte(SAVE_ADDR_BASE+12);//��ȡ���������ͱ��
		if(tp_dev.touchtype)										//X��Y��������Ļ�෴
		{
			CMD_RDX = 0x90;
			CMD_RDY = 0xD0;
		}else
		{
			CMD_RDX = 0xD0;
			CMD_RDY = 0x90;
		}
		return 1;
	}
	return 0;
}
const u8 TP_REMIND_MSG_TBL[] = "��ʮ�֣�ֱ������";//��ʾ�ַ���
/*.............................................................................
@Name		:	TP_Adj_Info_Show
@param	: u16 x0
					u16 y0
					u16 x1
					u16 y1
					u16 x2
					u16 y2
					u16 x3
					u16 y3
					u16 fac
@return	:	void
@Info		:	��ʾУ׼���
.............................................................................*/
void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac)
{
	POINT_COLOR = RED;
	Show_String08_NotBK(40, 160, "x1:");
	Show_String08_NotBK(40+80, 160, "y1:");
	Show_String08_NotBK(40, 180, "x2:");
	Show_String08_NotBK(40+80, 180, "y2:");
	Show_String08_NotBK(40, 200, "x3:");
	Show_String08_NotBK(40+80, 200, "y3:");
	Show_String08_NotBK(40, 220, "x4:");
	Show_String08_NotBK(40+80, 220, "y4:");
	Show_String08_NotBK(40, 240, "fac:");
	Show_Num(40+48, 160, x0);
	Show_Num(40+80+48, 160, y0);
	Show_Num(40+48, 180, x1);
	Show_Num(40+80+48, 180, y1);
	Show_Num(40+48, 200, x2);
	Show_Num(40+80+48, 200, y2);
	Show_Num(40+48, 220, x3);
	Show_Num(40+80+48, 220, y3);
	Show_Num(40+64, 240, fac);
}
/*.............................................................................
@Name		:	TP_Adjust
@Param	:	void
@return	:	void
@Info		:	������У׼����
.............................................................................*/
void TP_Adjust(void)
{
	u16 pos_temp[4][2];												//���껺��ֵ
	u8 cnt = 0;
	u16 d1, d2;
	u32 temp1, temp2;
	float fac;
	u16 outtime = 0;
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	LCD_Clear(WHITE);													//����
	POINT_COLOR = RED;
	LCD_Clear(WHITE);
	Show_HZ16_String_NotBK(5,40,(u8*)TP_REMIND_MSG_TBL);
																						//��ʾ��ʾ��Ϣ
	TP_Draw_Touch_Point(20, 20, RED);
	tp_dev.sta = 0;														//���������ź�
	tp_dev.xfac = 0;													//xfac��������Ƿ�У׼��������У׼֮ǰ����������������
	while(1)
	{
		tp_dev.scan(1);													//ɨ����������
		if((tp_dev.sta & 0xC0) == TP_CATH_PRES)	//����������һ�Σ���ʱ�����ɿ���
		{
			outtime = 0;
			tp_dev.sta &= ~(1<<6);								//��ǰ����Ѿ����������
			pos_temp[cnt][0] = tp_dev.x[0];
			pos_temp[cnt][1] = tp_dev.y[0];
			cnt++;
			switch(cnt)
			{
				case 1:
				{
					TP_Draw_Touch_Point(20, 20, WHITE);//�����1
					TP_Draw_Touch_Point(lcddev.width-20, 20, RED);
																						//����2
					break;
				}
				case 2:
				{
 					TP_Draw_Touch_Point(lcddev.width-20,20,WHITE);
																						//�����2
					TP_Draw_Touch_Point(20,lcddev.height-20,RED);	
																						//����3
					break;
				}
				case 3:
				{
 					TP_Draw_Touch_Point(20,lcddev.height-20,WHITE);			
																						//�����3
 					TP_Draw_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	
																						//����4
					break;				
				}
				case 4:
				{
					temp1 = abs(pos_temp[0][0] - pos_temp[1][0]);
																						//x1 - x2
					temp2 = abs(pos_temp[0][1] - pos_temp[1][1]);
																						//y1 - y2
					temp1 *= temp1;
					temp2 *= temp2;
					d1 = sqrt(temp1 + temp2);					//�õ�1��2�ľ���
					temp1 = abs(pos_temp[2][0] - pos_temp[3][0]);
																						//x3 - x4
					temp2 = abs(pos_temp[2][1] - pos_temp[3][1]);
																						//y3 - y4
					temp1 *= temp1;
					temp2 *= temp2;
					d2 = sqrt(temp1 + temp2);					//�õ�3��4�ľ���			
					fac = (float) d1/d2;
					if(fac<0.95 || fac>1.05 || d1 == 0 || d2 == 0)
																						//У׼�ĵ㲻�ϸ�
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//�����4
						TP_Draw_Touch_Point(20, 20, RED);//����1
						TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
						pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac*100);
																						//��ʾУ׼�Ľ��
						continue;
					}
					temp1 = abs(pos_temp[0][0] - pos_temp[2][0]);
																						//x1 - x3
					temp2 = abs(pos_temp[0][1] - pos_temp[2][1]);
																						//y1 - y3
					temp1 *= temp1;
					temp2 *= temp2;
					d1 = sqrt(temp1+temp2);						//�õ�1��3�ľ���
					temp1 = abs(pos_temp[1][0] - pos_temp[3][0]);
																						//x2 - x4
					temp2 = abs(pos_temp[1][1] - pos_temp[3][1]);
																						//y2 - y4
					temp1 *= temp1;
					temp2 *= temp2;
					d2 = sqrt(temp1+temp2);						//�õ�2��4�ľ���
					fac = (float)d1 / d2;
					if(fac<0.95 || fac>1.05)					//У׼�ĵ㲻�ϸ�
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//�����4
						TP_Draw_Touch_Point(20, 20, RED);//����1
						TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
						pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac*100);
																						//��ʾУ׼�Ľ��
						continue;
					}
					temp1 = abs(pos_temp[1][0] - pos_temp[2][0]);
																						//x1 - x3
					temp2 = abs(pos_temp[1][1] - pos_temp[2][1]);
																						//y1 - y3
					temp1 *= temp1;
					temp2 *= temp2;
					d1 = sqrt(temp1+temp2);						//�õ�1��4�ľ���					
					temp1 = abs(pos_temp[0][0] - pos_temp[3][0]);
																						//x1 - x3
					temp2 = abs(pos_temp[0][1] - pos_temp[3][1]);
																						//y1 - y3
					temp1 *= temp1;
					temp2 *= temp2;
					d2 = sqrt(temp1+temp2);						//�õ�2��3�ľ���			
					fac = (float)d1 / d2;
					if(fac<0.95 || fac>1.05)					//У׼�ĵ㲻�ϸ�
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//�����4
						TP_Draw_Touch_Point(20, 20, RED);//����1
						TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
						pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac*100);
																						//��ʾУ׼�Ľ��
						continue;
					}
					tp_dev.xfac = (float) (lcddev.width-40) / (pos_temp[1][0] - pos_temp[0][0]);
																						//�õ�xfac
					tp_dev.xoff = (lcddev.width-tp_dev.xfac*(pos_temp[1][0] + pos_temp[0][0])) / 2;
																						//�õ�xoff
					tp_dev.yfac = (float) (lcddev.height-40) / (pos_temp[2][1] - pos_temp[0][1]);
																						//�õ�yfac
					tp_dev.yoff = (lcddev.height-tp_dev.yfac*(pos_temp[2][1] + pos_temp[0][1])) / 2;
																						//�õ�yoff					
					if(abs(tp_dev.xfac)>2 || abs(tp_dev.yfac)>2)
																						//��������Ԥ����෴
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//�����4
						TP_Draw_Touch_Point(20, 20, RED);//�����1
						Show_String08_NotBK(40, 26, "TP Need ReadJust !");
						tp_dev.touchtype = ! tp_dev.touchtype;
																						//�޸Ĵ���������
						if(tp_dev.touchtype)						//X��Y��������Ļ�෴
						{
							CMD_RDX = 0x90;
							CMD_RDY = 0xD0;
						}else														//X,Y��������Ļ��ͬ
						{
							CMD_RDX = 0xD0;
							CMD_RDY = 0x90;
						}
						continue;
					}
					POINT_COLOR = BLUE;
					LCD_Clear(WHITE);									//����
					Show_String08_NotBK(35, 110, "Touch Screen Adjust OK!");
																						//У�����
					delay_ms(1000);
					TP_Save_Adjdata();								//����У׼����
					LCD_Clear(WHITE);									//����
					return;														//У�����
				}
			}
		}
		delay_ms(10);
		outtime ++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();											//�õ�������EEPROM�е�У׼ֵ
			break;
		}
	}
}
/*.............................................................................
@Name		:	TP_Init
@Param	:	void
@return	:	u8 ������ĻУ׼�Ľ�� 0 û�н���У׼  1 ���й�У׼
@Info		:	��ʼ��������
.............................................................................*/
u8 TP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	
																						//ʹ��PB��PD�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB14�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
																						// PB12��PB13��PB15�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//ͨ���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//D13�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	TP_Read_XY(&tp_dev.x[0], &tp_dev.y[0]);//��һ�ζ�ȡ��ʼ��
	if(TP_Get_Adjdata())											//�Ѿ�У׼
	{
		return 0;
	}else
	{
		LCD_Clear(WHITE);												//����
		TP_Adjust();														//��ĻУ׼
		TP_Save_Adjdata();											//����У׼����
	}
	TP_Get_Adjdata();
	return 1;	
}
/********************************************************************
*name:Test_TP
*description:��ʼ����Ļ����
*param:void
*return:void
********************************************************************/
void Test_Init(void)
{
	tp_dev.init();
	delay_ms(1500);
	LCD_Clear(WHITE);
	tp_dev.scan(0);							//��Ļɨ��
}
/********************************************************************
*name:Test_Touch
*description:���Դ�����
*param:void
*return:void
********************************************************************/
void Test_Touch(void)
{
	u8 key;
	while(1)
	{
		key = Get_Key();
		tp_dev.scan(0);						//ɨ�败����
		if(tp_dev.sta & TP_PRES_DOWN)//������������
		{
			if(tp_dev.x[0]<lcddev.width && tp_dev.y[0]<lcddev.height)
			{
				if(tp_dev.x[0]>(lcddev.width-24) && tp_dev.y[0]<16)
				{
					LCD_Clear(WHITE);
					POINT_COLOR = BLUE;
					Show_String08_NotBK(40, 40, "Press KEY0 to Adjust!");
				}else
				{
					TP_Draw_Big_Point(tp_dev.x[0], tp_dev.y[0], RED);
				}
			}
		}else
		{
			delay_ms(10);
		}
		if(key == 1)								//��������,ִ��У׼����						
		{
			LCD_Clear(WHITE);				//����
			TP_Adjust();						//��ĻУ׼
			TP_Save_Adjdata();			//����У׼���
			LCD_Clear(WHITE);
		}
	}
}
