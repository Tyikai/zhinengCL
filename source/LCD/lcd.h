#ifndef LCD_H_
#define LCD_H_
#include "sys.h"
#define LCD_BL	PDout(6)										//LCD背光 PD6
extern u16 POINT_COLOR;
extern u16 BACK_COLOR;
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 Xpos, u16 Ypos);
u16 LCD_ReadPoint(u16 x, u16 y);
void LCD_Clear(u16 color);
void LCD_Fill(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos, u16 color);
void LCD_DrawLine(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos);
void LCD_DrawRectangle(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos);
void LCD_DrawCircle(u16 Xpos, u16 Ypos, u8 r);
void LCD_Init(void);
/*.............................................................................
																	画笔颜色
.............................................................................*/	
#define WHITE			0xFFFF
#define BLACK     0x0000	  
#define BLUE      0x001F  
#define BRED      0XF81F
#define GRED 			0XFFE0
#define GBLUE			0X07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define GREEN     0x07E0
#define CYAN      0x7FFF
#define YELLOW    0xFFE0
#define BROWN 		0XBC40 										//棕色
#define BRRED 		0XFC07 										//棕红色
#define GRAY  		0X8430 										//灰色
#endif


