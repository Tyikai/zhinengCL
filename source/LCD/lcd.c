#include "lcd.h"
#include "ILI93xx.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_fsmc.h"
/*.............................................................................
												LCD的画笔颜色和背景色
.............................................................................*/	
u16 POINT_COLOR = 0x0000;										//画笔颜色
u16 BACK_COLOR = 0xFFFF;										//背景色
/*.............................................................................
@Name		:	LCD_SetCursor
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
@return	:	
@Info		:	设置光标位置
.............................................................................*/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	if(lcddev.id == 0x9341)										//ILI9341同其他芯片稍有不同
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos >> 8);
		LCD_WR_DATA(Xpos & 0xFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos >> 8);
		LCD_WR_DATA(Ypos & 0xFF);
	}else
	{
		if(lcddev.dir == 1)											//1横屏，调转X，y坐标
		{
			Xpos = lcddev.width -1 - Xpos;
		}
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}
}
/*.............................................................................
@Name		:	LCD_DrawPoint
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
@return	:
@Info		:	在屏幕中画点
.............................................................................*/
void LCD_DrawPoint(u16 Xpos, u16 Ypos)
{
	LCD_SetCursor(Xpos,Ypos);									//设置光标位置
	LCD_WriteRAM_Prepare();										//让GRAM准备就绪
	LCD->LCD_RAM = POINT_COLOR;
}
/*.............................................................................
@Name		:	LCD_ReadPoint
@Param	:	u16 Xpos 横坐标
					u16 Ypos 纵坐标
@return	:	u16 读取到的颜色值
@Info	:		读取指定GRAM地址的颜色值
.............................................................................*/
u16 LCD_ReadPoint(u16 x, u16 y)
{
	u16 r=0,g=0,b=0;
	if(x>=lcddev.width || y>= lcddev.height)	//超出GRAM的范围，退出
	{
		return 0;
	}
	LCD_SetCursor(x,y);												//设置光标
	if(lcddev.id == 0x9341)
	{
		LCD_WR_REG(0x2E);												//9341发送都GRAM指令
	}else
	{
		LCD_WR_REG(R34);												//R34写入数据到GRAM中
	}
	if(lcddev.id == 0x9320)										//ILI9320需要延时2μs
	{
		opt_delay(2);
	}
	if(LCD->LCD_RAM)
	{
		r = 0;
	}
	opt_delay(2);
	r = LCD->LCD_RAM;
	if(lcddev.id == 0x9341)										//ILI9341需要分两次读出
	{
		opt_delay(2);
		b = LCD->LCD_RAM;
		g = r & 0xFF;														//对于9341，第一次读取的是RG的值，R在前，G在后，各占8为
		g <<=8;
	}
																						//以下几款芯片直接返回颜色值
	if(lcddev.id == 0x9325 || lcddev.id == 0x4535 || lcddev.id == 0x4531 ||
		lcddev.id == 0x8989 || lcddev.id == 0xB505)
	{
		return r;
	}else if(lcddev.id == 0x9341)							//ILI9341需要公式转换
	{
		return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));
	}else
	{
		return LCD_BGR2RGB(r);
	}
}
/*.............................................................................
@Name		:	LCD_Clear
@Param	:	u16 color 填充的颜色
@return	:	void
@Info		:	清频函数
.............................................................................*/
void LCD_Clear(u16 color)
{
	u32 index = 0;
	u32 totalpoint = lcddev.width;
	totalpoint *= lcddev.height;							//得到像素点总数
	LCD_SetCursor(0x00,0x0000);								//设置光标位置
	LCD_WriteRAM_Prepare();										//开始写入GRAM
	for(index=0;index<totalpoint;index ++)
	{
		LCD->LCD_RAM = color;
	}
}
/*.............................................................................
@Name		:	LCD_Fill
@Param	:	u16 Xpos0 起始横坐标
					u16 Ypos0 起始纵坐标
					u16 Xpos	终止横坐标
					u16 Ypos	终止纵坐标
					u16 color 填充颜色
@return	:	void
@Info		:	在指定区域填充指定的颜色
.............................................................................*/
void LCD_Fill(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos, u16 color)
{
	u16 i,j;
	u16 xlen = Xpos - Xpos0 + 1;
	for(i=Ypos0;i<=Ypos;i++)
	{
		LCD_SetCursor(Xpos0,i);									//设置光标的位置
		LCD_WriteRAM_Prepare();									//开始写入GRAM
		for(j=0;j<xlen;j++)
		{
			LCD_WR_DATA(color);										//写入颜色
		}
	}
}
/*.............................................................................
@Name		:	LCD_DrawLine
@Param	:	u16 Xpos0 起始横坐标
					u16 Ypos0 起始纵坐标
					u16 Xpos	终止横坐标
					u16 Ypos	终止纵坐标
@return	:	void
@Info		:	画线
.............................................................................*/
void LCD_DrawLine(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos)
{
	int width = Xpos - Xpos0;
	int height = Ypos - Ypos0;
	u16 row = Xpos0;
	u16 col = Ypos0;
	int incx = -1;														//画线的方向
	int incy = -1;														//画线方向
	u16 lenth = 0;														//线的长度
	u16 i;
	u16 xerr=0,yerr=0;												//临时变量
	if(width > 0)		
	{
		incx = 1;																//正方向，向右
	}else if(width == 0)
	{
		incx = 0;																//垂直方向
	}else					
	{
		incx = -1;															//反方向，向左
		width = - width;
	}
	if(height > 0)
	{
		incy = 1;																//正方向，向下
	}else if(height == 0)
	{
		incy = 0;																//水平方向
	}else
	{
		incy = -1;															//反方向，向上
		height = - height;
	}
	if(width > height)												//确定两点之间的距离
	{
		lenth = width;
	}else
	{
		lenth = height;
	}
	for(i=0;i<=lenth + 1;i++)									//画线输出
	{
		LCD_DrawPoint(row, col);
		xerr += width;
		yerr += height;
		if(xerr > lenth)
		{
			xerr -= lenth;
			row += incx;
		}
		if(yerr > lenth)
		{
			yerr -= lenth;
			col += incy;
		}
	}
}
/*.............................................................................
@Name		:	LCD_DrawRectangle
@Param	:	u16 Xpos0 起始横坐标
					u16 Ypos0 起始纵坐标
					u16 Xpos	终止横坐标
					u16 Ypos	终止纵坐标
@return	:	void
@Info		:	绘制矩形
.............................................................................*/
void LCD_DrawRectangle(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos)
{
	LCD_DrawLine(Xpos0, Ypos0, Xpos, Ypos0);	//横
	LCD_DrawLine(Xpos0, Ypos0, Xpos0, Ypos);	//竖
	LCD_DrawLine(Xpos0, Ypos, Xpos, Ypos);		//横
	LCD_DrawLine(Xpos, Ypos0, Xpos, Ypos);		//竖
}
/*.............................................................................
@Name		:	LCD_DrawCircle
@Param	:	u16 Xpos 圆心横坐标
					u16 Ypos 圆心纵坐标
					u8 r		 圆半径
@return	:	void
@Info		:	绘制圆
.............................................................................*/
void LCD_DrawCircle(u16 Xpos, u16 Ypos, u8 r)
{
	u16 a = 0;
	u16 b = r;
	int di = 3 - (r<<1);
	while(a <= b)
	{
		LCD_DrawPoint(Xpos+a, Ypos-b);					//5
		LCD_DrawPoint(Xpos+b, Ypos-a);					//0
		LCD_DrawPoint(Xpos+b, Ypos+a);					//4
		LCD_DrawPoint(Xpos+a, Ypos+b);					//6
		LCD_DrawPoint(Xpos-a, Ypos+b);					//1
		LCD_DrawPoint(Xpos-b, Ypos+a);
		LCD_DrawPoint(Xpos-a, Ypos-b);					//2
		LCD_DrawPoint(Xpos-b, Ypos-a);					//7
		a++;
		if(di<0)																//使用Bresenham算法画圆
		{
			di += 4*a+6;
		}else
		{
			di += 10+4*(a-b);
			b--;
		}
	}
}
/*.............................................................................
@Name		:	LCD_Init
@Param	:	void
@return	:	void
@Info		:	初始化LCD屏幕
.............................................................................*/
void LCD_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	
																						//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);
																						//使能PORTB,D,E,G以及AFIO复用功能时钟
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//PD6 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	
																						//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;				 
																						//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 	
																						//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 	
	
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
	readWriteTiming.FSMC_DataSetupTime = 0x0f;// 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	//地址建立时间（ADDSET）为1个HCLK  
	writeTiming.FSMC_AddressHoldTime = 0x00;	//地址保持时间（A		
	writeTiming.FSMC_DataSetupTime = 0x03;		//数据保存时间为4个HCLK	
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 	
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//这里我们使用NE1 ，也就对应BTCR[0],[1]。
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; //不复用数据地址
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;//FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;//FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//存储器写使能
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; //读写使用不同的时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  //使能BANK1 	
 	delay_ms(50); 														// delay 50 ms 
 	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); 														// delay 50 ms 
  lcddev.id = LCD_ReadReg(0x0000); 
																						//读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300
  if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)
	{	
																						//尝试9341 ID的读取		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 													//dummy read 	
 		LCD_RD_DATA();   	    									//读到0X00
  	lcddev.id=LCD_RD_DATA();  							//读取93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA(); 							//读取41 	 		
 		if(lcddev.id!=0X9341)										//非9341,尝试是不是6804
		{	
 			LCD_WR_REG(0XBF);				   
			LCD_RD_DATA(); 												//dummy read 	 
	 		LCD_RD_DATA();   	    								//读回0X01			   
	 		LCD_RD_DATA(); 												//读回0XD0 			  	
	  	lcddev.id=LCD_RD_DATA();							//这里读回0X68 
			lcddev.id<<=8;
	  	lcddev.id|=LCD_RD_DATA();							//这里读回0X04	   	  
 		} 
		if(lcddev.id!=0X9341&&lcddev.id!=0X6804)lcddev.id=0x9341;//新增，用于识别9341 	     
	} 
	LCD_BL = 1;																//让屏幕亮
	//printf("当前LCD驱动芯片型号：%x\r\n",lcddev.id);
	if(lcddev.id==0X9341)											//9341初始化
	{	
		LCD_WR_REG(0xCF);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); //display on	
	}else if(lcddev.id==0x9325)//ILI9325
	{
		LCD_WriteReg(0x00E5,0x78F0); 
		LCD_WriteReg(0x0001,0x0100); 
		LCD_WriteReg(0x0002,0x0700); 
		LCD_WriteReg(0x0003,0x1030); 
		LCD_WriteReg(0x0004,0x0000); 
		LCD_WriteReg(0x0008,0x0202);  
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000); 
		LCD_WriteReg(0x000C,0x0000); 
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);
		//power on sequence VGHVGL
		LCD_WriteReg(0x0010,0x0000);   
		LCD_WriteReg(0x0011,0x0007);  
		LCD_WriteReg(0x0012,0x0000);  
		LCD_WriteReg(0x0013,0x0000); 
		LCD_WriteReg(0x0007,0x0000); 
		//vgh 
		LCD_WriteReg(0x0010,0x1690);   
		LCD_WriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout 
		LCD_WriteReg(0x0012,0x009D); //0x001b
		//delayms(100); 
		//vom amplitude
		LCD_WriteReg(0x0013,0x1900);
		//delayms(100); 
		//vom H
		LCD_WriteReg(0x0029,0x0025); 
		LCD_WriteReg(0x002B,0x000D); 
		//gamma
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0303);
		LCD_WriteReg(0x0032,0x0003);// 0006
		LCD_WriteReg(0x0035,0x0206);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0406); 
		LCD_WriteReg(0x0038,0x0304);//0200
		LCD_WriteReg(0x0039,0x0007); 
		LCD_WriteReg(0x003C,0x0602);// 0504
		LCD_WriteReg(0x003D,0x0008); 
		//ram
		LCD_WriteReg(0x0050,0x0000); 
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000); 
		LCD_WriteReg(0x0053,0x013F);  
		LCD_WriteReg(0x0060,0xA700); 
		LCD_WriteReg(0x0061,0x0001); 
		LCD_WriteReg(0x006A,0x0000); 
		//
		LCD_WriteReg(0x0080,0x0000); 
		LCD_WriteReg(0x0081,0x0000); 
		LCD_WriteReg(0x0082,0x0000); 
		LCD_WriteReg(0x0083,0x0000); 
		LCD_WriteReg(0x0084,0x0000); 
		LCD_WriteReg(0x0085,0x0000); 
		//
		LCD_WriteReg(0x0090,0x0010); 
		LCD_WriteReg(0x0092,0x0600); 
		
		LCD_WriteReg(0x0007,0x0133);
		LCD_WriteReg(0x00,0x0022);//
	}
	LCD_Display_Dir(0);		 										//默认为竖屏
	LCD_BL =1;																//点亮背光
	LCD_Clear(BKCOLOR);		
}