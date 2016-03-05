#include "showCH.h"
#include "ILI93xx.h"
#include "lcd.h"
#include "chinese.h"
#include "showEN.h"
/*.............................................................................
@Name	:	Show_HZ16
@Param	:	u16 Xpos ������
			u16 Ypos ������
			u8 *string ������ʾ������
			u8 mode ��ʾģʽ HAVE_BK   NO_BK
@return	:	void
@Info	:	��ʾ16*16����
.............................................................................*/
void Show_HZ16(u16 Xpos, u16 Ypos, u8 *string,u8 mode)
{
	u16 num;
	u16 t;
	u8 i,j;
	u8 temp;
	u16 x0 = Xpos;
	u16 colortemp = POINT_COLOR;
	num = sizeof(FONT16) / sizeof(type_GB16);//ͳ���ֿ�����������
	if(mode == HAVE_BK)
	{
		for(t=0;t<num;t++)
		{
			if((FONT16[t].Index[0] == *(string)) && 
				((FONT16[t].Index[1]) == *(string + 1)))
											//ͨ��ƥ�����ĵ����ź�λ��
			{
				for(i=0;i<32;i++)			//��ʾ�ú���
				{
					temp = FONT16[t].Marks[i];
					for(j=0;j<8;j++)		//���ֽڽ�����ʾ
					{
						if(temp & 0x80)		//��ʾ
						{
							POINT_COLOR = colortemp;
						}else
						{
							POINT_COLOR = BACK_COLOR;
						}
						LCD_DrawPoint(Xpos, Ypos);
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)//������Ļ��Χ���˳�
						{
							return;
						}
						if((Xpos-x0) == 16)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)//������Ļ��Χ
							{
								return;
							}
							break;			//�˳���ǰ���ֽ�
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
											//ͨ��ƥ�����ĵ����ź�λ��
			{
				for(i=0;i<32;i++)			//��ʾ�ú���
				{
					temp = FONT16[t].Marks[i];
					for(j=0;j<8;j++)		//���ֽڽ�����ʾ
					{
						if(temp & 0x80)		//��ʾ
						{
							LCD_DrawPoint(Xpos, Ypos);
						}
						temp <<= 1;
						Xpos++;
						if(Xpos>=lcddev.width)//������Ļ��Χ���˳�
						{
							return;
						}
						if((Xpos-x0) == 16)
						{
							Xpos = x0;
							Ypos++;
							if(Ypos>=lcddev.height)	//������Ļ��Χ
							{
								return;
							}
							break;			//�˳���ǰ���ֽ�
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
@Param	:	u16 Xpos ������
			u16 Ypos ������
			u8 *string ������ʾ������
@return	:	void
@Info	:	��ʾ16*16���֣�������ɫ
.............................................................................*/
void Show_HZ16_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ16(Xpos, Ypos, string, HAVE_BK);
}
/*.............................................................................
@Name	:	Show_16_NotBK
@Param	:	u16 Xpos ������
			u16 Ypos ������
			u8 *string ������ʾ������
@return	:	void
@Info	:	��ʾ16*16���֣��ޱ���ɫ
.............................................................................*/
void Show_HZ16_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	Show_HZ16(Xpos, Ypos, string, NO_BK);
}
/*.............................................................................
@Name	:	Show_HZ16_Stirng_BK
@Param	:	u16 Xpos ������
			u16 Ypos ������
			u8 *string ������ʾ������
@return	:	void
@Info	:	��ʾ16*16���ִ���������ɫ
.............................................................................*/
void Show_HZ16_String_BK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)						//ֱ���ַ���Ϊ��
	{
		if(fontFlag==EN)					//Ӣ���ַ������
		{
			if((*string) > 0x80)			//Ӣ���ַ�����Ҫu8���棬������0x80���ж�Ϊ����
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)			//���з�
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_BK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;					//�ַ����
				if(Xpos >= lcddev.width)	//������Ļ���
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ16_BK(Xpos, Ypos, string);
			string += 2;
			Xpos += 16;						//��һ���ַ�
		}
	}
}
/*.............................................................................
@Name	:	Show_HZ16_Stirng_NotBK
@Param	:	u16 Xpos ������
			u16 Ypos ������
			u8 *string ������ʾ������
@return	:	void
@Info	:	��ʾ16*16���ִ����ޱ���ɫ
.............................................................................*/
void Show_HZ16_String_NotBK(u16 Xpos, u16 Ypos, u8 *string)
{
	u16 x0 = Xpos;
	u8 fontFlag = EN;
	while((*string)!=0)						//ֱ���ַ���Ϊ��
	{
		if(fontFlag==EN)					//Ӣ���ַ������
		{
			if((*string) > 0x80)			//Ӣ���ַ�����Ҫu8���棬������0x80���ж�Ϊ����
			{
				fontFlag = CH;
			}else
			{
				if((*string) == 13)			//���з�
				{
					Ypos += 16;
					Xpos = x0;
					string++;
				}else
				{
					Show_String08_NotBK(Xpos, Ypos, string);
				}
				string++;
				Xpos += 8;					//�ַ����
				if(Xpos >= lcddev.width)	//������Ļ���
				{
					break;
				}
			}
		}else if(fontFlag == CH)
		{
			fontFlag = EN;
			Show_HZ16_NotBK(Xpos, Ypos, string);
			string += 2;
			Xpos += 16;						//��һ���ַ�
		}
	}
}