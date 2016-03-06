#include "lcd.h"
#include "ILI93xx.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_fsmc.h"
/*.............................................................................
												LCD�Ļ�����ɫ�ͱ���ɫ
.............................................................................*/	
u16 POINT_COLOR = 0x0000;										//������ɫ
u16 BACK_COLOR = 0xFFFF;										//����ɫ
/*.............................................................................
@Name		:	LCD_SetCursor
@Param	:	u16 Xpos ������
					u16 Ypos ������
@return	:	
@Info		:	���ù��λ��
.............................................................................*/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	if(lcddev.id == 0x9341)										//ILI9341ͬ����оƬ���в�ͬ
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos >> 8);
		LCD_WR_DATA(Xpos & 0xFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos >> 8);
		LCD_WR_DATA(Ypos & 0xFF);
	}else
	{
		if(lcddev.dir == 1)											//1��������תX��y����
		{
			Xpos = lcddev.width -1 - Xpos;
		}
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}
}
/*.............................................................................
@Name		:	LCD_DrawPoint
@Param	:	u16 Xpos ������
					u16 Ypos ������
@return	:
@Info		:	����Ļ�л���
.............................................................................*/
void LCD_DrawPoint(u16 Xpos, u16 Ypos)
{
	LCD_SetCursor(Xpos,Ypos);									//���ù��λ��
	LCD_WriteRAM_Prepare();										//��GRAM׼������
	LCD->LCD_RAM = POINT_COLOR;
}
/*.............................................................................
@Name		:	LCD_ReadPoint
@Param	:	u16 Xpos ������
					u16 Ypos ������
@return	:	u16 ��ȡ������ɫֵ
@Info	:		��ȡָ��GRAM��ַ����ɫֵ
.............................................................................*/
u16 LCD_ReadPoint(u16 x, u16 y)
{
	u16 r=0,g=0,b=0;
	if(x>=lcddev.width || y>= lcddev.height)	//����GRAM�ķ�Χ���˳�
	{
		return 0;
	}
	LCD_SetCursor(x,y);												//���ù��
	if(lcddev.id == 0x9341)
	{
		LCD_WR_REG(0x2E);												//9341���Ͷ�GRAMָ��
	}else
	{
		LCD_WR_REG(R34);												//R34д�����ݵ�GRAM��
	}
	if(lcddev.id == 0x9320)										//ILI9320��Ҫ��ʱ2��s
	{
		opt_delay(2);
	}
	if(LCD->LCD_RAM)
	{
		r = 0;
	}
	opt_delay(2);
	r = LCD->LCD_RAM;
	if(lcddev.id == 0x9341)										//ILI9341��Ҫ�����ζ���
	{
		opt_delay(2);
		b = LCD->LCD_RAM;
		g = r & 0xFF;														//����9341����һ�ζ�ȡ����RG��ֵ��R��ǰ��G�ں󣬸�ռ8Ϊ
		g <<=8;
	}
																						//���¼���оƬֱ�ӷ�����ɫֵ
	if(lcddev.id == 0x9325 || lcddev.id == 0x4535 || lcddev.id == 0x4531 ||
		lcddev.id == 0x8989 || lcddev.id == 0xB505)
	{
		return r;
	}else if(lcddev.id == 0x9341)							//ILI9341��Ҫ��ʽת��
	{
		return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));
	}else
	{
		return LCD_BGR2RGB(r);
	}
}
/*.............................................................................
@Name		:	LCD_Clear
@Param	:	u16 color ������ɫ
@return	:	void
@Info		:	��Ƶ����
.............................................................................*/
void LCD_Clear(u16 color)
{
	u32 index = 0;
	u32 totalpoint = lcddev.width;
	totalpoint *= lcddev.height;							//�õ����ص�����
	LCD_SetCursor(0x00,0x0000);								//���ù��λ��
	LCD_WriteRAM_Prepare();										//��ʼд��GRAM
	for(index=0;index<totalpoint;index ++)
	{
		LCD->LCD_RAM = color;
	}
}
/*.............................................................................
@Name		:	LCD_Fill
@Param	:	u16 Xpos0 ��ʼ������
					u16 Ypos0 ��ʼ������
					u16 Xpos	��ֹ������
					u16 Ypos	��ֹ������
					u16 color �����ɫ
@return	:	void
@Info		:	��ָ���������ָ������ɫ
.............................................................................*/
void LCD_Fill(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos, u16 color)
{
	u16 i,j;
	u16 xlen = Xpos - Xpos0 + 1;
	for(i=Ypos0;i<=Ypos;i++)
	{
		LCD_SetCursor(Xpos0,i);									//���ù���λ��
		LCD_WriteRAM_Prepare();									//��ʼд��GRAM
		for(j=0;j<xlen;j++)
		{
			LCD_WR_DATA(color);										//д����ɫ
		}
	}
}
/*.............................................................................
@Name		:	LCD_DrawLine
@Param	:	u16 Xpos0 ��ʼ������
					u16 Ypos0 ��ʼ������
					u16 Xpos	��ֹ������
					u16 Ypos	��ֹ������
@return	:	void
@Info		:	����
.............................................................................*/
void LCD_DrawLine(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos)
{
	int width = Xpos - Xpos0;
	int height = Ypos - Ypos0;
	u16 row = Xpos0;
	u16 col = Ypos0;
	int incx = -1;														//���ߵķ���
	int incy = -1;														//���߷���
	u16 lenth = 0;														//�ߵĳ���
	u16 i;
	u16 xerr=0,yerr=0;												//��ʱ����
	if(width > 0)		
	{
		incx = 1;																//����������
	}else if(width == 0)
	{
		incx = 0;																//��ֱ����
	}else					
	{
		incx = -1;															//����������
		width = - width;
	}
	if(height > 0)
	{
		incy = 1;																//����������
	}else if(height == 0)
	{
		incy = 0;																//ˮƽ����
	}else
	{
		incy = -1;															//����������
		height = - height;
	}
	if(width > height)												//ȷ������֮��ľ���
	{
		lenth = width;
	}else
	{
		lenth = height;
	}
	for(i=0;i<=lenth + 1;i++)									//�������
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
@Param	:	u16 Xpos0 ��ʼ������
					u16 Ypos0 ��ʼ������
					u16 Xpos	��ֹ������
					u16 Ypos	��ֹ������
@return	:	void
@Info		:	���ƾ���
.............................................................................*/
void LCD_DrawRectangle(u16 Xpos0, u16 Ypos0, u16 Xpos, u16 Ypos)
{
	LCD_DrawLine(Xpos0, Ypos0, Xpos, Ypos0);	//��
	LCD_DrawLine(Xpos0, Ypos0, Xpos0, Ypos);	//��
	LCD_DrawLine(Xpos0, Ypos, Xpos, Ypos);		//��
	LCD_DrawLine(Xpos, Ypos0, Xpos, Ypos);		//��
}
/*.............................................................................
@Name		:	LCD_DrawCircle
@Param	:	u16 Xpos Բ�ĺ�����
					u16 Ypos Բ��������
					u8 r		 Բ�뾶
@return	:	void
@Info		:	����Բ
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
		if(di<0)																//ʹ��Bresenham�㷨��Բ
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
@Info		:	��ʼ��LCD��Ļ
.............................................................................*/
void LCD_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	
																						//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);
																						//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//PD6 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	
																						//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;				 
																						//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 	
																						//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 	
	
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	readWriteTiming.FSMC_DataSetupTime = 0x0f;// ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	//��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
	writeTiming.FSMC_AddressHoldTime = 0x00;	//��ַ����ʱ�䣨A		
	writeTiming.FSMC_DataSetupTime = 0x03;		//���ݱ���ʱ��Ϊ4��HCLK	
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 	
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//��������ʹ��NE1 ��Ҳ�Ͷ�ӦBTCR[0],[1]��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; //���������ݵ�ַ
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;//FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;//FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//�洢��дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; //��дʹ�ò�ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  //ʹ��BANK1 	
 	delay_ms(50); 														// delay 50 ms 
 	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); 														// delay 50 ms 
  lcddev.id = LCD_ReadReg(0x0000); 
																						//����ID����ȷ,����lcddev.id==0X9300�жϣ���Ϊ9341��δ����λ������»ᱻ����9300
  if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)
	{	
																						//����9341 ID�Ķ�ȡ		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 													//dummy read 	
 		LCD_RD_DATA();   	    									//����0X00
  	lcddev.id=LCD_RD_DATA();  							//��ȡ93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA(); 							//��ȡ41 	 		
 		if(lcddev.id!=0X9341)										//��9341,�����ǲ���6804
		{	
 			LCD_WR_REG(0XBF);				   
			LCD_RD_DATA(); 												//dummy read 	 
	 		LCD_RD_DATA();   	    								//����0X01			   
	 		LCD_RD_DATA(); 												//����0XD0 			  	
	  	lcddev.id=LCD_RD_DATA();							//�������0X68 
			lcddev.id<<=8;
	  	lcddev.id|=LCD_RD_DATA();							//�������0X04	   	  
 		} 
		if(lcddev.id!=0X9341&&lcddev.id!=0X6804)lcddev.id=0x9341;//����������ʶ��9341 	     
	} 
	LCD_BL = 1;																//����Ļ��
	//printf("��ǰLCD����оƬ�ͺţ�%x\r\n",lcddev.id);
	if(lcddev.id==0X9341)											//9341��ʼ��
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
	LCD_Display_Dir(0);		 										//Ĭ��Ϊ����
	LCD_BL =1;																//��������
	LCD_Clear(BKCOLOR);		
}