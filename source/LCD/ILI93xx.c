#include "ILI93xx.h"
#include "sys.h"
#include "delay.h"
_lcd_dev lcddev;							//����LCD��Ҫ����
/*.............................................................................
@Name	:	LCD_WR_REG
@Param	:	u16 regval	�Ĵ���ֵ
@return	:	void
@Info	:	��LCDд������
.............................................................................*/
void LCD_WR_REG(u16 regval)
{
	LCD->LCD_REG = regval;
}
/*.............................................................................
@Name	:	LCD_WR_DATA
@Param	:	u16 data
@return	:	void
@Info	:	��LCDд������
.............................................................................*/
void LCD_WR_DATA(u16 data)
{
	LCD->LCD_RAM = data;
}
/*.............................................................................
@Name	:	LCD_RD_DATA
@Param	:	void
@return	:	u16 ��ȡ������
@Info	:	��LCD�ж�ȡ����
.............................................................................*/
u16 LCD_RD_DATA(void)
{
	return LCD->LCD_RAM;
}
/*.............................................................................
@Name	:	LCD_WriteReg
@Param	:	u16 LCD_reg ָ���ļĴ�����ַ
			u16 LCD_regval ��д������
@return	:	void
@Info	:	������д��ָ���ļĴ���
.............................................................................*/
void LCD_WriteReg(u16 LCD_reg, u16 LCD_regval)
{
	LCD->LCD_REG = LCD_reg;
	LCD->LCD_RAM = LCD_regval;
}
/*.............................................................................
@Name	:	LCD_ReadReg
@Param	:	u16 LCD_reg ָ���ļĴ�����ַ
@return	:	u16 �Ĵ����е�����
@Info	:	��ȡָ���Ĵ���
.............................................................................*/
u16 LCD_ReadReg(u16 LCD_reg)
{
	LCD_WR_REG(LCD_reg);					//��LCDд������
	delay_us(5);
	return LCD_RD_DATA();					//��ȡLCD�е�����
}
/*.............................................................................
@Name	:	LCD_WriteRAM_Prepare
@Param	:	void
@return	:	void
@Info	:	��GRAM׼������
.............................................................................*/
void LCD_WriteRAM_Prepare(void)
{
	LCD->LCD_REG = lcddev.wramcmd;
}
/*.............................................................................
@Name	:	LCD_WriteRAM
@Param	:	u16 RGB_Code
@return	:	void
@Info	:	��GRAMд������
.............................................................................*/
void LCD_WriteRAM(u16 RGB_Code)
{
	LCD->LCD_RAM = RGB_Code;
}
/*.............................................................................
@Name	:	LCD_BGR2RGB
@Param	:	u16 BGR ��ɫֵ
@return	:	u16 ת������ɫֵ
@Info	:	��GRAMд������
.............................................................................*/
u16 LCD_BGR2RGB(u16 BGR)
{
	u16 r,g,b,rgb;
	b = (BGR>>0) & 0x1F;					//0x1F=0000 0000 0001 1111��5λΪ��ɫ
	g = (BGR>>5) & 0x3F;					//0x3F=0000 0000 0011 1111�м�6λΪ��ɫ
	r = (BGR>>11) & 0x1F;					//0x1F=0000 0000 0001 1111���5λΪ��ɫ
	rgb = (b<<11) + (g<<5) + (r<<0);
	return (rgb);
}
/*.............................................................................
@Name	:	opt_delay
@Param	:	u8 i ��ʱ
@return	:	void
@Info	:	ʱ���Ż�ʱ��Ҫ
.............................................................................*/
void opt_delay(u8 i)
{
	while(i--);
}
/*.............................................................................
@Name	:	LCD_Display_Dir
@Param	:	u8 dir ��Ļ���� 0����,  1����
@return	:	void
@Info	:	����LCD��ʾ����
.............................................................................*/
void LCD_Display_Dir(u8 dir)
{
	if(dir == 0)							//����
	{
		lcddev.dir = 0;						//����
		lcddev.width = 240;
		lcddev.height = 320;
		if(lcddev.id == 0x9341)
		{
			lcddev.wramcmd = 0x2C;
			lcddev.setxcmd = 0x2A;
			lcddev.setycmd = 0x2B;
		}else								//�������͵�����оƬ
		{
			lcddev.wramcmd = R34;
			lcddev.setxcmd = R32;
			lcddev.setycmd = R33;
		}
	}else if(dir == 1)						// ����
	{
		lcddev.dir = 1;						//����
		lcddev.width = 320;
		lcddev.height = 240;
		if(lcddev.id == 0x9341)
		{
			lcddev.wramcmd = 0x2C;
			lcddev.setxcmd = 0x2A;
			lcddev.setycmd = 0x2B;
		}else								//�������͵�����оƬ
		{
			lcddev.wramcmd = R34;
			lcddev.setxcmd = R33;
			lcddev.setycmd = R32;
		}
	}
											//�趨Ĭ��ɨ�跽��
	LCD_Scan_Dir(DFT_SCAN_DIR);
}
/*.............................................................................
@Name	:	LCD_Scan_Dir
@Param	:	u8 dir ɨ�跽��
@return	:	void
@Info	:	����LCD�Զ�ɨ�跽��
.............................................................................*/
void LCD_Scan_Dir(u8 dir)
{
	u8 regval = 0;
	u8 dirreg = 0;
	u16 temp;
	if(lcddev.id == 0x9341)
	{
		switch(dir)
		{
			case L2R_U2D:					//������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U:					//������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D:					//���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U:					//���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R:					//���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L:					//���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R:					//���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L:					//���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;
		}
		dirreg = 0x36;
		regval |= 0x08;
		LCD_WriteReg(dirreg,regval);
 		if(regval&0X20)
		{
			if(lcddev.width<lcddev.height)	//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)	//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}  
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);		
	}else
	{
		switch(dir)
		{
			case L2R_U2D:					//������,���ϵ���
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U:					//������,���µ���
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D:					//���ҵ���,���ϵ���
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U:					//���ҵ���,���µ���
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R:					//���ϵ���,������
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L:					//���ϵ���,���ҵ���
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R:					//���µ���,������
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L:					//���µ���,���ҵ���
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}	
	}
}
