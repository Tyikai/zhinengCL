#ifndef SHOWCH_H_
#define SHOWCH_H_
#include "sys.h"
#define FONT_TYPE_EN_12 12									//ascii 1206字库
#define FONT_TYPE_EN_16	16									//ascii 1616字库
#define HAVE_BK 0														//显示模式
#define NO_BK	1															//显示模式
#define EN 0																//英文字符串标志
#define CH 1																//中文字符串标志
void Show_HZ16(u16 Xpos, u16 Ypos, u8 *string,u8 mode);
void Show_HZ16_BK(u16 Xpos, u16 Ypos, u8 *string);
void Show_HZ16_NotBK(u16 Xpos, u16 Ypos, u8 *string);
void Show_HZ16_String_BK(u16 Xpos, u16 Ypos, u8 *string);
void Show_HZ16_String_NotBK(u16 Xpos, u16 Ypos, u8 *string);

#endif

