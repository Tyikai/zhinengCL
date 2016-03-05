#include "showCH.h"
#include "ILI93xx.h"
#include "lcd.h"
#include "chinese.h"
#include "showEN.h"
/*.............................................................................
@Name	:	Show_HZ16
@Param	:	u16 Xpos 横坐标
			u16 Ypos 纵坐标
			u8 *string 中文显示的数组
			u8 mode 显示模式 HAVE_BK   NO_BK
@return	:	void
@Info	:	显示16*16汉字
.............................................................................*/
void Show_HZ16(u16 Xpos, u16 Ypos, u8 *string,u8 mode)
{
	u16 num;
	u16 t;
	u8 i,j;
	u8 temp;
	u16 x0 = Xpos;
	u16 colortemp = POINT_COLOR;
	num = sizeof(FONT16) / sizeof(type_GB16);//统计字库中字体总数
	if(mode == HAVE_BK)
	{
		for(t=0;t<num;t++)
		{
			if((FONT16[t].Index[0] == *(string)) && 
				((FONT16[t].Index[1]) == *(string + 1)))
											//通过匹配中文的区号和位号
			{
				for(i=0;i<32;i++)			//显示该汉字
				{
					temp = FONT16[t].Marks[i];
					for(j=0;j<8;j++)		//按字节进行显示
					{
						if(temp & 0x80)		//显示
						{
							POINT_COLOR = colortemp;
						}else
						{
							POINT_COLOR = BACK_COLOR;
						}
						LCD_DrawPoint(Xpos, Ypos);
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)//超出屏幕范围，退出
						{
							return;
						}
						if((Xpos-x0) == 16)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)//超出屏幕范围
							{
								return;
							}
							break;			//退出当前的字节
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
				for(i=0;i<32;i++)			//显示该汉字
				{
					temp = FONT16[t].Marks[i];
					for(j=0;j<8;j++)		//按字节进行显示
					{
						if(temp & 0x80)		//显示
						{
							LCD_DrawPoint(Xpos, Ypos);
						}
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)//超出屏幕范围，退出
						{
							return;
						}
						if((Xpos-x0) == 16)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)	//超出屏幕范围
							{
								return;
							}
							break;			//退出当前的字节
						}
					}
				}
			}
		}	
	}
	POINT_COLOR = colortemp;
}
/*.............................................................................
@Name	:	Show_16_BK
@Param	:	u16 Xpos 横坐标
			u16 Ypos 纵坐标
			u8 *string 中文显示的数组
@return	:	void
@Info	:	显示16*16汉字，带背景色
.............................................................................*/
void Show_HZ16_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ16(Xpos, Ypos, string, HAVE_BK);
}
/*.............................................................................
@Name	:	Show_16_NotBK
@Param	:	u16 Xpos 横坐标
			u16 Ypos 纵坐标
			u8 *string 中文显示的数组
@return	:	void
@Info	:	显示16*16汉字，无背景色
.............................................................................*/
void Show_HZ16_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ16(Xpos, Ypos, string, NO_BK);
}
/*.............................................................................
@Name	:	Show_HZ16_Stirng_BK
@Param	:	u16 Xpos 横坐标
			u16 Ypos 纵坐标
			u8 *string 中文显示的数组
@return	:	void
@Info	:	显示16*16汉字串，带背景色
.............................................................................*/
void Show_HZ16_String_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)						//直到字符串为空
	{
		if(fontFlag==EN)					//英文字符的情况
		{
			if((*string) > 0x80)			//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)			//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_BK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;					//字符间隔
				if(Xpos >= lcddev.width)	//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ16_BK(Xpos, Ypos, string);
			string += 2;
			Xpos += 16;						//下一个字符
		}
	}
}
/*.............................................................................
@Name	:	Show_HZ16_Stirng_NotBK
@Param	:	u16 Xpos 横坐标
			u16 Ypos 纵坐标
			u8 *string 中文显示的数组
@return	:	void
@Info	:	显示16*16汉字串，无背景色
.............................................................................*/
void Show_HZ16_String_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)						//直到字符串为空
	{
		if(fontFlag==EN)					//英文字符的情况
		{
			if((*string) > 0x80)			//英文字符仅需要u8储存，若大于0x80则判定为中文
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)			//换行符
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_NotBK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;					//字符间隔
				if(Xpos >= lcddev.width)	//超出屏幕宽度
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ16_NotBK(Xpos, Ypos, string);
			string += 2;
			Xpos += 16;						//下一个字符
		}
	}
}