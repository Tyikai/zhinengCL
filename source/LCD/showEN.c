#include "showEN.h"
#include "showCH.h"
#include "ILI93xx.h"
#include "lcd.h"
#include "english.h"
/*.............................................................................
@Name		:	Show_Char1206
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 character 待显示的字符
					u8 mode 显示模式 HAVE_BK  NO_BK
@return	:	void
@Info		:	显示12*06的ASCII字符
.............................................................................*/
void Show_Char1206(u16 Xpos, u16 Ypos, u8 character, u8 mode)
{
	u8 t;
	u8 m;
	u8 temp;	
	u16 colortemp = POINT_COLOR;
	u16 y0 = Ypos;
	character = character - ' ';
	if(mode == HAVE_BK)												//有背景色
	{
		for(t=0;t<12;t++)
		{
			temp = ascii_1206[character][t];
			for(m=0;m<8;m++)											//以一个字节的单位进行显示
			{
				if(temp & 0x80)
				{
					POINT_COLOR = colortemp;
				}else
				{
					POINT_COLOR = BACK_COLOR;
				}
				LCD_DrawPoint(Xpos, Ypos);					//显示
				temp <<= 1;													//向左移动一位
				Ypos++;															//行自增
				if(Xpos >= lcddev.width)						//超出屏幕的高度，退出
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 12)									//写完一行
				{
					Ypos = y0;
					Xpos++;														//列自增
					if(Xpos >= lcddev.width)					//超出屏幕
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//转到下一行
				}
			}
		}
	}else																			//无背景色
	{
		for(t=0;t<12;t++)
		{
			temp = ascii_1206[character][t];
			for(m=0;m<8;m++)											//以一个字节的单位进行显示
			{
				if(temp & 0x80)
				{
					LCD_DrawPoint(Xpos, Ypos);				//显示
				}
				temp <<=1;													//向左移动一位
				Ypos++;															//行自增
				if(Xpos>=lcddev.height)							//超出屏幕的高度，退出
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 12)									//写完一行
				{
					Ypos = y0;
					Xpos++;														//列自增
					if(Xpos >= lcddev.width)					//超出屏幕
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//转到下一行
				}
			}
		}		
	}
	POINT_COLOR = colortemp;
}
/*.............................................................................
@Name		:	Show_Char1608
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 character 待显示的字符
					u8 mode 显示模式 HAVE_BK  NO_BK
@return	:	void
@Info		:	显示16*08的ASCII字符
.............................................................................*/
void Show_Char1608(u16 Xpos, u16 Ypos, u8 character, u8 mode)
{
	u8 t;
	u8 m;
	u8 temp;	
	u16 colortemp = POINT_COLOR;
	u16 y0 = Ypos;
	character = character - ' ';
	if(mode == HAVE_BK)												//有背景
	{
		for(t=0;t<16;t++)
		{
			temp = ascii_1608[character][t];
			for(m=0;m<8;m++)											//以一个字节的单位进行显示
			{
				if(temp & 0x80)
				{
					POINT_COLOR = colortemp;
				}else
				{
					POINT_COLOR = BACK_COLOR;
				}
				LCD_DrawPoint(Xpos, Ypos);					//显示
				temp <<=1;													//向左移动一位
				Ypos++;															//行自增
				if(Xpos >= lcddev.width)						//超出屏幕的高度，退出
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 16)									//写完一行
				{
					Ypos = y0;
					Xpos++;														//列自增
					if(Xpos >= lcddev.width)					//超出屏幕
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//转到下一行
				}
			}
		}	
	}else																			//无背景
	{
		for(t=0;t<16;t++)
		{
			temp = ascii_1608[character][t];
			for(m=0;m<8;m++)											//以一个字节的单位进行显示
			{
				if(temp & 0x80)
				{
					LCD_DrawPoint(Xpos, Ypos);				//显示
				}
				temp <<=1;													//向左移动一位
				Ypos++;															//行自增
				if(Xpos >= lcddev.width)						//超出屏幕的高度，退出
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 16)									//写完一行
				{
					Ypos = y0;
					Xpos++;														//列自增
					if(Xpos >= lcddev.width)					//超出屏幕
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//转到下一行
				}
			}
		}	
	}
	POINT_COLOR = colortemp;
}
/*.............................................................................
@Name		:	Show_Char1206_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 character
@return	:	void
@Info		:	显示12*06的ASCII字符,带背景色
.............................................................................*/
void Show_Char1206_BK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1206(Xpos, Ypos, character, HAVE_BK);
}
/*.............................................................................
@Name		:	Show_Char1206_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 character
@return	:	void
@Info		:	显示12*06的ASCII字符,无背景色
.............................................................................*/
void Show_Char1206_NotBK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1206(Xpos, Ypos, character, NO_BK);
}
/*.............................................................................
@Name		:	Show_Char1206_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 character
@return	:	void
@Info		:	显示16*08的ASCII字符,带背景色
.............................................................................*/
void Show_Char1608_BK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1608(Xpos, Ypos, character, HAVE_BK);
}
/*.............................................................................
@Name		:	Show_Char1608_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 character
@return	:	void
@Info		:	显示16*08的ASCII字符,无背景色
.............................................................................*/
void Show_Char1608_NotBK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1608(Xpos, Ypos, character, NO_BK);
}
/*.............................................................................
@Name		:	Show_String06_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *str 待显示的字符串
@return	:	void
@Info		:	显示12*06字符串，带背景色
.............................................................................*/
void Show_String06_BK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0 = Xpos;
	while((*str <='~') && (*str >= ' '))			//判断是否属于ascii字符
	{
		Show_Char1206_BK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//超出屏幕的宽度，换行
		{
			Xpos = x0;
			Ypos += 12;
			if(Ypos >= lcddev.height)							//如果超出屏幕范围，退出
			{
				break;
			}
		}
		Xpos += 12 / 2; 												//完成一个字符
		str ++;																	//下一个字符
	}
}
/*.............................................................................
@Name		:	Show_String06_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *str 待显示的字符串
@return	:	void
@Info		:	显示12*06字符串,无背景色
.............................................................................*/
void Show_String06_NotBK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0= Xpos;
	while((*str <='~') && (*str >= ' '))					//判断是否属于ascii字符
	{
		Show_Char1206_NotBK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//超出屏幕的宽度，换行
		{
			Xpos = x0;
			Ypos += 12;
			if(Ypos >= lcddev.height)							//如果超出屏幕范围，退出
			{
				break;
			}
		}
		Xpos += 12 / 2; 												//完成一个字符
		str ++;																		//下一个字符
	}
}
/*.............................................................................
@Name		:	Show_String08_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *str 待显示的字符串
@return	:	void
@Info		:	显示16*08字符串，带背景色
.............................................................................*/
void Show_String08_BK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0 = Xpos;
	while((*str <='~') && (*str>= ' '))				//判断是否属于ascii字符
	{
		Show_Char1608_BK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//超出屏幕的宽度，换行
		{
			Xpos = x0;
			Ypos += 16;
			if(Ypos >= lcddev.height)							//如果超出屏幕范围，退出
			{
				break;
			}
		}		
		Xpos += 16 / 2; 												//完成一个字符
		str ++;																	//下一个字符
	}
}
/*.............................................................................
@Name		:	Show_String08_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *str 待显示的字符串
@return	:	void
@Info		:	显示16*08字符串，无背景色
.............................................................................*/
void Show_String08_NotBK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0 = Xpos;
	while((*str <='~') && (*str>= ' '))				//判断是否属于ascii字符
	{
		Show_Char1608_NotBK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//超出屏幕的宽度，换行
		{
			Xpos = x0;
			Ypos += 16;
			if(Ypos >= lcddev.height)							//如果超出屏幕范围，退出
			{
				break;
			}
		}	
		Xpos += 16 / 2; 												//完成一个字符
		str ++;																	//下一个字符
	}
}
/*.............................................................................
@Name		:	Show_Num
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u32 num 显示数字
@return	:	void
@Info		: 显示数字
.............................................................................*/
void Show_Num(u16 Xpos, u16 Ypos, u32 num)
{
	u8 t, temp;
	u8 len=0;																	//纪录数据的长度
	u32 tempnum = num;
	u8 flag=0;
	do
	{
		tempnum = tempnum / 10;
		len ++;
	}while(tempnum!=0);
	for(t=0;t<len;t++)
	{
		temp = (num / MNPower(10, len-t-1)) % 10;//获取第n位的数字
		if((flag==0)&&(t<(len-1)))
		{
			if(temp == 0)
			{
				Show_Char1608_BK(Xpos+8*t,Ypos,' ');
				continue;
			}else
			{
				flag = 1;
			}
		}
		Show_Char1608_BK(Xpos+8*t, Ypos, temp+'0');
	}
}
/*.............................................................................
@Name		:	MNPower
@Param	:	u8 m 基数
					u8 n 指数
@return	:	u32 M^N 返回 M^N的结果
@Info		: 用于获取第N位的数字，主要用于计算分离各个位
					temp = (num / (MNPower(10,len-1-t))) % 10;//获取第n位的数字
.............................................................................*/
u32 MNPower(u8 m, u8 n)
{
	u32 resoult = 1;
	while(n--)
	{
		resoult *= m;
	}
	return resoult;
}
