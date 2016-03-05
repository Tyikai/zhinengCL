#include "ILI93xx.h"
#include "sys.h"
#include "delay.h"
_lcd_dev lcddev;							//管理LCD重要参数
/*.............................................................................
@Name	:	LCD_WR_REG
@Param	:	u16 regval	寄存器值
@return	:	void
@Info	:	向LCD写入命令
.............................................................................*/
void LCD_WR_REG(u16 regval)
{
	LCD->LCD_REG = regval;
}
/*.............................................................................
@Name	:	LCD_WR_DATA
@Param	:	u16 data
@return	:	void
@Info	:	向LCD写入数据
.............................................................................*/
void LCD_WR_DATA(u16 data)
{
	LCD->LCD_RAM = data;
}
/*.............................................................................
@Name	:	LCD_RD_DATA
@Param	:	void
@return	:	u16 读取到数据
@Info	:	从LCD中读取数据
.............................................................................*/
u16 LCD_RD_DATA(void)
{
	return LCD->LCD_RAM;
}
/*.............................................................................
@Name	:	LCD_WriteReg
@Param	:	u16 LCD_reg 指定的寄存器地址
			u16 LCD_regval 待写入数据
@return	:	void
@Info	:	将数据写入指定的寄存器
.............................................................................*/
void LCD_WriteReg(u16 LCD_reg, u16 LCD_regval)
{
	LCD->LCD_REG = LCD_reg;
	LCD->LCD_RAM = LCD_regval;
}
/*.............................................................................
@Name	:	LCD_ReadReg
@Param	:	u16 LCD_reg 指定的寄存器地址
@return	:	u16 寄存器中的数据
@Info	:	读取指定寄存器
.............................................................................*/
u16 LCD_ReadReg(u16 LCD_reg)
{
	LCD_WR_REG(LCD_reg);					//向LCD写入命令
	delay_us(5);
	return LCD_RD_DATA();					//读取LCD中的数据
}
/*.............................................................................
@Name	:	LCD_WriteRAM_Prepare
@Param	:	void
@return	:	void
@Info	:	让GRAM准备就绪
.............................................................................*/
void LCD_WriteRAM_Prepare(void)
{
	LCD->LCD_REG = lcddev.wramcmd;
}
/*.............................................................................
@Name	:	LCD_WriteRAM
@Param	:	u16 RGB_Code
@return	:	void
@Info	:	向GRAM写入数据
.............................................................................*/
void LCD_WriteRAM(u16 RGB_Code)
{
	LCD->LCD_RAM = RGB_Code;
}
/*.............................................................................
@Name	:	LCD_BGR2RGB
@Param	:	u16 BGR 颜色值
@return	:	u16 转换后颜色值
@Info	:	向GRAM写入数据
.............................................................................*/
u16 LCD_BGR2RGB(u16 BGR)
{
	u16 r,g,b,rgb;
	b = (BGR>>0) & 0x1F;					//0x1F=0000 0000 0001 1111低5位为蓝色
	g = (BGR>>5) & 0x3F;					//0x3F=0000 0000 0011 1111中间6位为绿色
	r = (BGR>>11) & 0x1F;					//0x1F=0000 0000 0001 1111最高5位为红色
	rgb = (b<<11) + (g<<5) + (r<<0);
	return (rgb);
}
/*.............................................................................
@Name	:	opt_delay
@Param	:	u8 i 延时
@return	:	void
@Info	:	时间优化时需要
.............................................................................*/
void opt_delay(u8 i)
{
	while(i--);
}
/*.............................................................................
@Name	:	LCD_Display_Dir
@Param	:	u8 dir 屏幕方向 0竖屏,  1横屏
@return	:	void
@Info	:	设置LCD显示方向
.............................................................................*/
void LCD_Display_Dir(u8 dir)
{
	if(dir == 0)							//竖屏
	{
		lcddev.dir = 0;						//竖屏
		lcddev.width = 240;
		lcddev.height = 320;
		if(lcddev.id == 0x9341)
		{
			lcddev.wramcmd = 0x2C;
			lcddev.setxcmd = 0x2A;
			lcddev.setycmd = 0x2B;
		}else								//其他类型的驱动芯片
		{
			lcddev.wramcmd = R34;
			lcddev.setxcmd = R32;
			lcddev.setycmd = R33;
		}
	}else if(dir == 1)						// 横屏
	{
		lcddev.dir = 1;						//横屏
		lcddev.width = 320;
		lcddev.height = 240;
		if(lcddev.id == 0x9341)
		{
			lcddev.wramcmd = 0x2C;
			lcddev.setxcmd = 0x2A;
			lcddev.setycmd = 0x2B;
		}else								//其他类型的驱动芯片
		{
			lcddev.wramcmd = R34;
			lcddev.setxcmd = R33;
			lcddev.setycmd = R32;
		}
	}
											//设定默认扫描方向
	LCD_Scan_Dir(DFT_SCAN_DIR);
}
/*.............................................................................
@Name	:	LCD_Scan_Dir
@Param	:	u8 dir 扫描方向
@return	:	void
@Info	:	设置LCD自动扫描方向
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
			case L2R_U2D:					//从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U:					//从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D:					//从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U:					//从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R:					//从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L:					//从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R:					//从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L:					//从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;
		}
		dirreg = 0x36;
		regval |= 0x08;
		LCD_WriteReg(dirreg,regval);
 		if(regval&0X20)
		{
			if(lcddev.width<lcddev.height)	//交换X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)	//交换X,Y
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
			case L2R_U2D:					//从左到右,从上到下
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U:					//从左到右,从下到上
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D:					//从右到左,从上到下
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U:					//从右到左,从下到上
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R:					//从上到下,从左到右
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L:					//从上到下,从右到左
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R:					//从下到上,从左到右
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L:					//从下到上,从右到左
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}	
	}
}
