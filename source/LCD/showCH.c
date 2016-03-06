#include "showCH.h"
#include "ILI93xx.h"
#include "lcd.h"
#include "chinese.h"
#include "showEN.h"
/*.............................................................................
@Name		:	Show_HZ16
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
					u8 mode 显示模式 HAVE_BK   NO_BK
@return	:	void
@Info		:	显示16*16汉字
.............................................................................*/
void Show_HZ16(u16 Xpos, u16 Ypos, u8 *string,u8 mode)
{
	u16 num;
	u16 t;
	u8 i,j;
	u8 temp;
	u16 x0 = Xpos;
	u16 colortemp = POINT_COLOR;
	num = sizeof(FONT16) / sizeof(type_GB16);	//统计字库中字体总数
	if(mode == HAVE_BK)
	{
		for(t=0;t<num;t++)
		{
			if((FONT16[t].Index[0] == *(string)) && 
				((FONT16[t].Index[1]) == *(string + 1)))
																						//通过匹配中文的区号和位号
			{
				for(i=0;i<32;i++)										//显示该汉字
				{
					temp = FONT16[t].Marks[i];
					for(j=0;j<8;j++)									//按字节进行显示
					{
						if(temp & 0x80)									//显示
						{
							POINT_COLOR = colortemp;
						}else
						{
							POINT_COLOR = BACK_COLOR;
						}
						LCD_DrawPoint(Xpos, Ypos);
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)					//超出屏幕范围，退出
						{
							return;
						}
						if((Xpos-x0) == 16)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)				//超出屏幕范围
							{
								return;
							}
							break;												//退出当前的字节
						}
					}
				}
			}
		}
	}else
	{
		for(t=0;t<num;t++)
		{
			if((FONT16[t].Index[0] == *(string)) && 
				((FONT16[t].Index[1]) == *(string + 1)))
																						//通过匹配中文的区号和位号
			{
				for(i=0;i<32;i++)										//显示该汉字
				{
					temp = FONT16[t].Marks[i];
					for(j=0;j<8;j++)									//按字节进行显示
					{
						if(temp & 0x80)									//显示
						{
							LCD_DrawPoint(Xpos, Ypos);
						}
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)					//超出屏幕范围，退出
						{
							return;
						}
						if((Xpos-x0) == 16)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)				//超出屏幕范围
							{
								return;
							}
							break;												//退出当前的字节
						}
					}
				}
			}
		}	
	}
	POINT_COLOR = colortemp;
}
/*.............................................................................
@Name		:	Show_16_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示16*16汉字，带背景色
.............................................................................*/
void Show_HZ16_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ16(Xpos, Ypos, string, HAVE_BK);
}
/*.............................................................................
@Name		:	Show_16_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示16*16汉字，无背景色
.............................................................................*/
void Show_HZ16_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ16(Xpos, Ypos, string, NO_BK);
}
/*.............................................................................
@Name		:	Show_HZ16_Stirng_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示16*16汉字串，带背景色
.............................................................................*/
void Show_HZ16_String_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)												//直到字符串为空
	{
		if(fontFlag==EN)												//英文字符的情况
		{
			if((*string) > 0x80)									//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)									//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_BK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;													//字符间隔
				if(Xpos >= lcddev.width)						//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ16_BK(Xpos, Ypos, string);
			string += 2;
			Xpos += 16;														//下一个字符
		}
	}
}
/*.............................................................................
@Name		:	Show_HZ16_Stirng_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示16*16汉字串，无背景色
.............................................................................*/
void Show_HZ16_String_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)												//直到字符串为空
	{
		if(fontFlag==EN)												//英文字符的情况
		{
			if((*string) > 0x80)									//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)									//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_NotBK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;													//字符间隔
				if(Xpos >= lcddev.width)						//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ16_NotBK(Xpos, Ypos, string);
			string += 2;
			Xpos += 16;														//下一个字符
		}
	}
}

/*.............................................................................
@Name		:	Show_HZ24_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示24汉字，带背景色
.............................................................................*/
void Show_HZ24_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ24(Xpos, Ypos, string, HAVE_BK);
}
/*.............................................................................
@Name		:	Show_HZ24_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示24*24汉字，无背景色
.............................................................................*/
void Show_HZ24_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ24(Xpos, Ypos, string, NO_BK);
}
/*.............................................................................
@Name		:	Show_HZ24_Stirng_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示24汉字串，带背景色
.............................................................................*/
void Show_HZ24_String_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)												//直到字符串为空
	{
		if(fontFlag==EN)												//英文字符的情况
		{
			if((*string) > 0x80)									//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)									//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_BK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;													//字符间隔
				if(Xpos>=lcddev.width)							//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ24_BK(Xpos, Ypos, string);
			string += 2;
			Xpos += 28;															//下一个字符
		}
	}
}
/*.............................................................................
@Name		:	Show_HZ24_Stirng_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示24*24汉字串，无背景色
.............................................................................*/
void Show_HZ24_String_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)												//直到字符串为空
	{
		if(fontFlag==EN)												//英文字符的情况
		{
			if((*string) > 0x80)									//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)									//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_NotBK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;													//字符间隔
				if(Xpos>=lcddev.width)							//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ24_NotBK(Xpos, Ypos, string);
			string += 2;
			Xpos += 28;														//下一个字符
		}
	}
}
/*.............................................................................
@Name		:	Show_HZ48
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
					u8 mode 显示模式 HAVE_BK   NO_BK
@return	:	void
@Info		:	显示48*48汉字
.............................................................................*/
void Show_HZ48(u16 Xpos, u16 Ypos, u8 *string,u8 mode)
{
	u16 num;
	u16 t;
	u16 i,j;
	u8 temp;
	u16 x0 = Xpos;
	u16 colortemp = POINT_COLOR;
	num = sizeof(FONT48) / sizeof(type_GB48);	//统计字库中字体总数
	if(mode == HAVE_BK)
	{
		for(t=0;t<num;t++)
		{
			if((FONT48[t].Index[0] == *(string)) && 
				((FONT48[t].Index[1]) == *(string + 1)))//通过匹配中文的区号和位号
			{
				for(i=0;i<288;i++)										//显示该汉字
				{
					temp = FONT48[t].Marks[i];
					for(j=0;j<8;j++)									//按字节进行显示
					{
						if(temp & 0x80)									//显示
						{
							POINT_COLOR = colortemp;
						}else
						{
							POINT_COLOR = BACK_COLOR;
						}
						LCD_DrawPoint(Xpos, Ypos);
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)					//超出屏幕范围，退出
						{
							return;
						}
						if((Xpos-x0) == 48)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)				//超出屏幕范围
							{
								return;
							}
							break;												//退出当前的字节
						}
					}
				}
			}
		}
	}else
	{
		for(t=0;t<num;t++)
		{
			if((FONT48[t].Index[0] == *(string)) && 
				((FONT48[t].Index[1]) == *(string + 1)))	//通过匹配中文的区号和位号
			{
				for(i=0;i<288;i++)										//显示该汉字
				{
					temp = FONT48[t].Marks[i];
					for(j=0;j<8;j++)									//按字节进行显示
					{
						if(temp & 0x80)									//显示
						{
							LCD_DrawPoint(Xpos, Ypos);
						}
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)					//超出屏幕范围，退出
						{
							return;
						}
						if((Xpos-x0) == 48)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)				//超出屏幕范围
							{
								return;
							}
							break;												//退出当前的字节
						}
					}
				}
			}
		}	
	}
	POINT_COLOR = colortemp;
}
/*.............................................................................
@Name		:	Show_HZ48_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示48*48汉字，带背景色
.............................................................................*/
void Show_HZ48_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ48(Xpos, Ypos, string, HAVE_BK);
}
/*.............................................................................
@Name		:	Show_HZ48_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示48*48汉字，无背景色
.............................................................................*/
void Show_HZ48_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ48(Xpos, Ypos, string, NO_BK);
}
/*.............................................................................
@Name		:	Show_HZ48_Stirng_BK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示48*48汉字串，带背景色
.............................................................................*/
void Show_HZ48_String_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)												//直到字符串为空
	{
		if(fontFlag==EN)												//英文字符的情况
		{
			if((*string) > 0x80)									//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)									//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_BK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;													//字符间隔
				if(Xpos>=lcddev.width)							//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ24_BK(Xpos, Ypos, string);
			string += 2;
			Xpos += 48;															//下一个字符
		}
	}
}
/*.............................................................................
@Name		:	Show_HZ48_Stirng_NotBK
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string 中文显示的数组
@return	:	void
@Info		:	显示48*48汉字串，无背景色
.............................................................................*/
void Show_HZ48_String_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)												//直到字符串为空
	{
		if(fontFlag==EN)												//英文字符的情况
		{
			if((*string) > 0x80)									//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)									//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_NotBK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;													//字符间隔
				if(Xpos>=lcddev.width)							//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ24_NotBK(Xpos, Ypos, string);
			string += 2;
			Xpos += 48;														//下一个字符
		}
	}
}
/*.............................................................................
@Name		:	Get_Bit
@Param	:	u8 byte 用于显示的字节数组元素
					u16 bit 需要显示的位
@return	:	u8 返回显示结果 1 显示  0 不显示
@Info		:	通过对字库中数组的每一位进行判断，用以显示
.............................................................................*/
u8 Get_Bit(u8 byte, u16 bit)
{
	if(bit<0 || bit>7)												//不是显示范围
	{
		return 0;
	}
	return (byte>>bit)&0x01;									
}
/*.............................................................................
@Name		:	Get_Font24
@Param	:	u8 *string 需要查找的中文24*24
@return	:	u16 返回该中文在字库中的地址
@Info		: 从自定义的字库中提取该汉字的地址
.............................................................................*/
u16 Get_Font24(u8 *string)
{
	u16 num;
	u16 t;
	num = sizeof(FONT48) / sizeof(type_GB48);	//统计字库中字体总数
	for(t=0;t<num;t++)
	{
		if((FONT48[t].Index[0] == *(string)) && ((FONT48[t].Index[1]) == *(string + 1)))
																						//通过匹配中文的区号和位号
		{
			break;
		}
	}
	if(t>num)																	//t大于num，则该字符不存在
	{
		return 0;
	}else
	{
		return t;
	}
}
/*.............................................................................
@Name		:	Show_HZ24
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
					u8 *string　带显示的字符
					u8 mode 显示模式 1 HAVE_BK 0 NO_BK
@return	:	u8 返回显示结果 1 显示  0 不显示
@Info		:	通过对字库中数组的每一位进行判断，用以显示
.............................................................................*/
void Show_HZ24(u16 Xpos, u16 Ypos, u8 *string,u8 mode)
{
	
	u16 num;
	u16 t;
	u16 i,j;
	u8 temp;
	u16 x0 = Xpos;
	u16 colortemp = POINT_COLOR;
	num = Get_Font24(string);
	if(!num)																	//如果字库中无字符，退出 
	{
		return; 
	}
	if(mode == HAVE_BK)
	{
		for(i=0;i<3;i++)
		{
			for(t=0;t<8;t++)
			{
				if(Get_Bit(FONT48[num].Marks[i],7-t))
				{
					POINT_COLOR = colortemp;					//显示
				}else
				{
					POINT_COLOR = BACK_COLOR;					//不显示
				}
				LCD_DrawPoint(Xpos, Ypos);					//描绘点
				Xpos++;
				if(Xpos>=lcddev.width)							//超出屏幕范围，退出
				{
					return;
				}
				if((Xpos-x0) == 24)
				{
					Xpos = x0;
					Ypos++;
					if(Ypos>=lcddev.height)						//超出屏幕范围
					{
						return;
					}
					break;														//退出当前的字节
				}				
			}
		}
	}else
	{
		for(j=0;j<24;j++)
		{
			for(i=0;i<3;i++)
			{
				for(t=0;t<8;t++)
				{
					if(Get_Bit(FONT48[num].Marks[j*i],7-t))
					{
						LCD_DrawPoint(Xpos, Ypos);					//描绘点
					}
					Xpos++;
					if(Xpos>=lcddev.width)							//超出屏幕范围，退出
					{
						return;
					}
					if((Xpos-x0) == 24)
					{
						Xpos = x0;
						Ypos++;
						if(Ypos>=lcddev.height)						//超出屏幕范围
						{
							return;
						}
						break;														//退出当前的字节
					}				
				}
			}
		}		
	}
	POINT_COLOR = colortemp;
}
