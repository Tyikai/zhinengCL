#ifndef TOUCH_H_
#define TOUCH_H_
#include "sys.h"
#define TP_PRES_DOWN	0x80									//触摸屏被触摸
#define TP_CATH_PRES	0x40									//有按键按下了,校准用   
/*.............................................................................
														电阻屏芯片连接引脚
.............................................................................*/																
#define PEN  		PDin(13)  									//PD13 		PEN
#define DOUT 		PBin(14)   									//PB14 	 	MISO
#define TDIN 		PBout(15)  									//PB15  	MOSI
#define TCLK 		PBout(13)  									//PB13  	SCLK
#define TCS  		PBout(12)  									//PB12  	CS  
typedef struct
{
	u8 ( *init)(void);												//初始化触摸屏控制器,函数指针指向 u8 TP_Init(void)
	u8 ( *scan)(u8);													//扫描触摸屏, 0 屏幕扫描， 1 物理坐标 ,函数指针 u8 TP_Scan(u8 tp)
	void ( *adjust)(void);										// 触摸屏校准, 函数指针 u8 TP_Adjust(void)
	u16 x[5];																	//原始坐标(第一次按下时的坐标)
	u16 y[5];											
	u8 sta;																		//笔的状态 [7] 0 松开  1 按下 [6] 0 无按键按下  1 有按键按下
	float xfac;																//				触摸屏校准参数
	float yfac;
	short xoff;
	short yoff;
	u8 touchtype;															//[0] 0 竖屏 1 横屏 [6:1] 保留 [7] 0 电阻屏 1 电容屏
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

