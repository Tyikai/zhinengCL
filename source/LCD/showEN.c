#include "showEN.h"
#include "showCH.h"
#include "ILI93xx.h"
#include "lcd.h"
#include "english.h"
/*.............................................................................
@Name		:	Show_Char1206
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 character ����ʾ���ַ�
					u8 mode ��ʾģʽ HAVE_BK  NO_BK
@return	:	void
@Info		:	��ʾ12*06��ASCII�ַ�
.............................................................................*/
void Show_Char1206(u16 Xpos, u16 Ypos, u8 character, u8 mode)
{
	u8 t;
	u8 m;
	u8 temp;	
	u16 colortemp = POINT_COLOR;
	u16 y0 = Ypos;
	character = character - ' ';
	if(mode == HAVE_BK)												//�б���ɫ
	{
		for(t=0;t<12;t++)
		{
			temp = ascii_1206[character][t];
			for(m=0;m<8;m++)											//��һ���ֽڵĵ�λ������ʾ
			{
				if(temp & 0x80)
				{
					POINT_COLOR = colortemp;
				}else
				{
					POINT_COLOR = BACK_COLOR;
				}
				LCD_DrawPoint(Xpos, Ypos);					//��ʾ
				temp <<= 1;													//�����ƶ�һλ
				Ypos++;															//������
				if(Xpos >= lcddev.width)						//������Ļ�ĸ߶ȣ��˳�
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 12)									//д��һ��
				{
					Ypos = y0;
					Xpos++;														//������
					if(Xpos >= lcddev.width)					//������Ļ
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//ת����һ��
				}
			}
		}
	}else																			//�ޱ���ɫ
	{
		for(t=0;t<12;t++)
		{
			temp = ascii_1206[character][t];
			for(m=0;m<8;m++)											//��һ���ֽڵĵ�λ������ʾ
			{
				if(temp & 0x80)
				{
					LCD_DrawPoint(Xpos, Ypos);				//��ʾ
				}
				temp <<=1;													//�����ƶ�һλ
				Ypos++;															//������
				if(Xpos>=lcddev.height)							//������Ļ�ĸ߶ȣ��˳�
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 12)									//д��һ��
				{
					Ypos = y0;
					Xpos++;														//������
					if(Xpos >= lcddev.width)					//������Ļ
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//ת����һ��
				}
			}
		}		
	}
	POINT_COLOR = colortemp;
}
/*.............................................................................
@Name		:	Show_Char1608
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 character ����ʾ���ַ�
					u8 mode ��ʾģʽ HAVE_BK  NO_BK
@return	:	void
@Info		:	��ʾ16*08��ASCII�ַ�
.............................................................................*/
void Show_Char1608(u16 Xpos, u16 Ypos, u8 character, u8 mode)
{
	u8 t;
	u8 m;
	u8 temp;	
	u16 colortemp = POINT_COLOR;
	u16 y0 = Ypos;
	character = character - ' ';
	if(mode == HAVE_BK)												//�б���
	{
		for(t=0;t<16;t++)
		{
			temp = ascii_1608[character][t];
			for(m=0;m<8;m++)											//��һ���ֽڵĵ�λ������ʾ
			{
				if(temp & 0x80)
				{
					POINT_COLOR = colortemp;
				}else
				{
					POINT_COLOR = BACK_COLOR;
				}
				LCD_DrawPoint(Xpos, Ypos);					//��ʾ
				temp <<=1;													//�����ƶ�һλ
				Ypos++;															//������
				if(Xpos >= lcddev.width)						//������Ļ�ĸ߶ȣ��˳�
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 16)									//д��һ��
				{
					Ypos = y0;
					Xpos++;														//������
					if(Xpos >= lcddev.width)					//������Ļ
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//ת����һ��
				}
			}
		}	
	}else																			//�ޱ���
	{
		for(t=0;t<16;t++)
		{
			temp = ascii_1608[character][t];
			for(m=0;m<8;m++)											//��һ���ֽڵĵ�λ������ʾ
			{
				if(temp & 0x80)
				{
					LCD_DrawPoint(Xpos, Ypos);				//��ʾ
				}
				temp <<=1;													//�����ƶ�һλ
				Ypos++;															//������
				if(Xpos >= lcddev.width)						//������Ļ�ĸ߶ȣ��˳�
				{
					POINT_COLOR = colortemp;
					return;
				}
				if((Ypos-y0) == 16)									//д��һ��
				{
					Ypos = y0;
					Xpos++;														//������
					if(Xpos >= lcddev.width)					//������Ļ
					{
						POINT_COLOR = colortemp;
						return;
					}
					break;														//ת����һ��
				}
			}
		}	
	}
	POINT_COLOR = colortemp;
}
/*.............................................................................
@Name		:	Show_Char1206_BK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 character
@return	:	void
@Info		:	��ʾ12*06��ASCII�ַ�,������ɫ
.............................................................................*/
void Show_Char1206_BK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1206(Xpos, Ypos, character, HAVE_BK);
}
/*.............................................................................
@Name		:	Show_Char1206_NotBK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 character
@return	:	void
@Info		:	��ʾ12*06��ASCII�ַ�,�ޱ���ɫ
.............................................................................*/
void Show_Char1206_NotBK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1206(Xpos, Ypos, character, NO_BK);
}
/*.............................................................................
@Name		:	Show_Char1206_BK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 character
@return	:	void
@Info		:	��ʾ16*08��ASCII�ַ�,������ɫ
.............................................................................*/
void Show_Char1608_BK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1608(Xpos, Ypos, character, HAVE_BK);
}
/*.............................................................................
@Name		:	Show_Char1608_NotBK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 character
@return	:	void
@Info		:	��ʾ16*08��ASCII�ַ�,�ޱ���ɫ
.............................................................................*/
void Show_Char1608_NotBK(u16 Xpos, u16 Ypos, u8 character)
{
	Show_Char1608(Xpos, Ypos, character, NO_BK);
}
/*.............................................................................
@Name		:	Show_String06_BK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 *str ����ʾ���ַ���
@return	:	void
@Info		:	��ʾ12*06�ַ�����������ɫ
.............................................................................*/
void Show_String06_BK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0 = Xpos;
	while((*str <='~') && (*str >= ' '))			//�ж��Ƿ�����ascii�ַ�
	{
		Show_Char1206_BK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//������Ļ�Ŀ�ȣ�����
		{
			Xpos = x0;
			Ypos += 12;
			if(Ypos >= lcddev.height)							//���������Ļ��Χ���˳�
			{
				break;
			}
		}
		Xpos += 12 / 2; 												//���һ���ַ�
		str ++;																	//��һ���ַ�
	}
}
/*.............................................................................
@Name		:	Show_String06_NotBK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 *str ����ʾ���ַ���
@return	:	void
@Info		:	��ʾ12*06�ַ���,�ޱ���ɫ
.............................................................................*/
void Show_String06_NotBK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0= Xpos;
	while((*str <='~') && (*str >= ' '))					//�ж��Ƿ�����ascii�ַ�
	{
		Show_Char1206_NotBK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//������Ļ�Ŀ�ȣ�����
		{
			Xpos = x0;
			Ypos += 12;
			if(Ypos >= lcddev.height)							//���������Ļ��Χ���˳�
			{
				break;
			}
		}
		Xpos += 12 / 2; 												//���һ���ַ�
		str ++;																		//��һ���ַ�
	}
}
/*.............................................................................
@Name		:	Show_String08_BK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 *str ����ʾ���ַ���
@return	:	void
@Info		:	��ʾ16*08�ַ�����������ɫ
.............................................................................*/
void Show_String08_BK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0 = Xpos;
	while((*str <='~') && (*str>= ' '))				//�ж��Ƿ�����ascii�ַ�
	{
		Show_Char1608_BK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//������Ļ�Ŀ�ȣ�����
		{
			Xpos = x0;
			Ypos += 16;
			if(Ypos >= lcddev.height)							//���������Ļ��Χ���˳�
			{
				break;
			}
		}		
		Xpos += 16 / 2; 												//���һ���ַ�
		str ++;																	//��һ���ַ�
	}
}
/*.............................................................................
@Name		:	Show_String08_NotBK
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u8 *str ����ʾ���ַ���
@return	:	void
@Info		:	��ʾ16*08�ַ������ޱ���ɫ
.............................................................................*/
void Show_String08_NotBK(u16 Xpos, u16 Ypos, u8 *str)
{
	u16 x0 = Xpos;
	while((*str <='~') && (*str>= ' '))				//�ж��Ƿ�����ascii�ַ�
	{
		Show_Char1608_NotBK(Xpos, Ypos, *str);
		if(Xpos >= lcddev.width)								//������Ļ�Ŀ�ȣ�����
		{
			Xpos = x0;
			Ypos += 16;
			if(Ypos >= lcddev.height)							//���������Ļ��Χ���˳�
			{
				break;
			}
		}	
		Xpos += 16 / 2; 												//���һ���ַ�
		str ++;																	//��һ���ַ�
	}
}
/*.............................................................................
@Name		:	Show_Num
@Param	:	u16 Xpos ������
					u16 Ypos ������
					u32 num ��ʾ����
@return	:	void
@Info		: ��ʾ����
.............................................................................*/
void Show_Num(u16 Xpos, u16 Ypos, u32 num)
{
	u8 t, temp;
	u8 len=0;																	//��¼���ݵĳ���
	u32 tempnum = num;
	u8 flag=0;
	do
	{
		tempnum = tempnum / 10;
		len ++;
	}while(tempnum!=0);
	for(t=0;t<len;t++)
	{
		temp = (num / MNPower(10, len-t-1)) % 10;//��ȡ��nλ������
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
@Param	:	u8 m ����
					u8 n ָ��
@return	:	u32 M^N ���� M^N�Ľ��
@Info		: ���ڻ�ȡ��Nλ�����֣���Ҫ���ڼ���������λ
					temp = (num / (MNPower(10,len-1-t))) % 10;//��ȡ��nλ������
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
