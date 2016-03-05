#ifndef SHOWEN_H_
#define SHOWEN_H_
#include "sys.h"
void Show_Char1206(u16 Xpos, u16 Ypos, u8 character, u8 mode);
void Show_Char1608(u16 Xpos, u16 Ypos, u8 character, u8 mode);
void Show_Char1206_BK(u16 Xpos, u16 Ypos, u8 character);
void Show_Char1206_NotBK(u16 Xpos, u16 Ypos, u8 character);
void Show_Char1608_BK(u16 Xpos, u16 Ypos, u8 character);
void Show_Char1608_NotBK(u16 Xpos, u16 Ypos, u8 character);
void Show_String06_BK(u16 Xpos, u16 Ypos, u8 *str);
void Show_String06_NotBK(u16 Xpos, u16 Ypos, u8 *str);
void Show_String08_BK(u16 Xpos, u16 Ypos, u8 *str);
void Show_String08_NotBK(u16 Xpos, u16 Ypos, u8 *str);
void Show_Num(u16 Xpos, u16 Ypos, u32 num);
u32 MNPower(u8 m, u8 n);
#endif

