#ifndef TOUCH_H_
#define TOUCH_H_
#include "sys.h"
#define TP_PRES_DOWN	0x80									//������������
#define TP_CATH_PRES	0x40									//�а���������,У׼��   
/*.............................................................................
														������оƬ��������
.............................................................................*/																
#define PEN  		PDin(13)  									//PD13 		PEN
#define DOUT 		PBin(14)   									//PB14 	 	MISO
#define TDIN 		PBout(15)  									//PB15  	MOSI
#define TCLK 		PBout(13)  									//PB13  	SCLK
#define TCS  		PBout(12)  									//PB12  	CS  
typedef struct
{
	u8 ( *init)(void);												//��ʼ��������������,����ָ��ָ�� u8 TP_Init(void)
	u8 ( *scan)(u8);													//ɨ�败����, 0 ��Ļɨ�裬 1 �������� ,����ָ�� u8 TP_Scan(u8 tp)
	void ( *adjust)(void);										// ������У׼, ����ָ�� u8 TP_Adjust(void)
	u16 x[5];																	//ԭʼ����(��һ�ΰ���ʱ������)
	u16 y[5];											
	u8 sta;																		//�ʵ�״̬ [7] 0 �ɿ�  1 ���� [6] 0 �ް�������  1 �а�������
	float xfac;																//				������У׼����
	float yfac;
	short xoff;
	short yoff;
	u8 touchtype;															//[0] 0 ���� 1 ���� [6:1] ���� [7] 0 ������ 1 ������
}_m_tp_dev;
void TP_Write_Byte(u8 data);
u16 TP_Read_AD(u8 cmd);
u16 TP_Read_XOY(u8 cmdxy);
u8 TP_Read_XY(u16 *x, u16 *y);
u8 TP_Read_XY2(u16 *x, u16 *y);
void TP_Draw_Touch_Point(u16 x, u16 y, u16 color);
void TP_Draw_Big_Point(u16 x, u16 y, u16 color);
u8 TP_Scan(u8 tp);
void TP_Save_Adjdata(void);
u8 TP_Get_Adjdata(void);
void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac);
void TP_Adjust(void);
u8 TP_Init(void);
void Test_Init(void);
void Test_Touch(void);
#endif

