#include "touch.h"
#include "delay.h"
#include "lcd.h"
#include "24Cxx.h"
#include "ShowEN.h"
#include "ShowCH.h"
#include "ILI93xx.h"
#include "math.h"
#include "stdlib.h"
#include "key.h"
#include "key.h"
#define READ_TIMES		5											//读取次数
#define LOST_VAL			1											//丢失值
#define ERR_RANGE			50										//误差范围
u8 CMD_RDX = 0xD0;													//读取X坐标指令
u8 CMD_RDY = 0x90;													//读取Y坐标指令
_m_tp_dev tp_dev=
{
	TP_Init,																	//初始化触摸屏控制器,函数指针指向 u8 TP_Init(void)
	TP_Scan,																	//扫描触摸屏, 0 屏幕扫描， 1 物理坐标 ,函数指针 u8 TP_Scan(u8 tp)
	TP_Adjust,																// 触摸屏校准, 函数指针 u8 TP_Adjust(void)
	0,																				//原始坐标(第一次按下时的坐标)
	0,
	0,																				//笔的状态 [7] 0 松开  1 按下 [6] 0 无按键按下  1 有按键按下
	0,																				//				触摸屏校准参数
	0,
	0,
	0,
	0,																				//触摸屏的类型																
};
extern _m_tp_dev tp_dev;
/*.............................................................................
@Name		:	TP_Write_Byte
@Param	:	u8 data		待写入的数据
@return	:	void
@Info		:	通过(模拟)SPI向触摸屏控制IC写入1byte数据
.............................................................................*/
void TP_Write_Byte(u8 data)
{
	u8 t = 0;
	for(t=0;t<8;t++)
	{
		if(data & 0x80)
		{
			TDIN = 1;
		}else
		{
			TDIN = 0;
		}
		data <<= 1;
		TCLK = 0;
		TCLK = 1;																//上升沿有效
	}
}
/*.............................................................................
@Name		:	TP_Read_AD(u8 cmd)
@Param	:	u8 cmd	指令
@return	:	u16 读取到的adc值
@Info		:	通过(模拟)SPI读取触摸屏控制器IC的adc值
.............................................................................*/
u16 TP_Read_AD(u8 cmd)
{
	u8 t=0;
	u16 data = 0;
	TCLK = 0;																	//先拉低时钟
	TDIN = 0;																	//拉低数据线
	TCS = 0;																	//选中触摸屏IC
	TP_Write_Byte(cmd);												//发送命令字
	delay_us(6);															//ADS7856的转换时间最长为6μs
	TCLK = 0;
	delay_us(1);
	TCLK = 1;																	//给1个时钟,清除BUSY
	TCLK = 0;
	for(t=0;t<16;t++)
	{
		data <<= 1;
		TCLK = 0;																//下降沿有效
		TCLK = 1;
		if(DOUT)
		{
			data ++;
		}
	}
	data >>= 4;																//只有高12位有效
	TCS = 1;																	//释放片选
	return data;
}
/*.............................................................................
@Name		:	TP_Read_XOY
@Param	:	u8 cmdxy RDX指令或RDY指令
@return	:	u16 读取到的坐标值
@Info		:	读取一个坐标值 X或Y
.............................................................................*/
u16 TP_Read_XOY(u8 cmdxy)
{
	u16 i, j;
	u16 buf[READ_TIMES];											//储存读取到的值
	u16 sum = 0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{
		buf[i] = TP_Read_AD(cmdxy);
	}
	for(i=0;i<READ_TIMES-1;i++)								//升序排列
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
	{
		sum += buf[i];
	}
	temp = sum / (READ_TIMES-2*LOST_VAL);
	return temp;
}
/*.............................................................................
@Name		:	TP_Read_XY
@Param	:	u16 *x	x坐标
					u16 *y  y坐标
@return	:	u8返回读取结果 0 失败  1 成功
@Info		:	读取(X,Y)坐标  读取的x，y值小于50判定为读取失败
.............................................................................*/
u8 TP_Read_XY(u16 *x, u16 *y)
{
	u16 xtemp,ytemp;
	xtemp = TP_Read_XOY(CMD_RDX);							//读取x坐标
	ytemp = TP_Read_XOY(CMD_RDY);							//读取y坐标
	*x = xtemp;
	*y = ytemp;
	return 1;
}
/*.............................................................................
@Name		:	TP_Read_XY2
@Param	:	u16 *x	读取到的x坐标
					u16 *y	读取到的y坐标
@return	:	u8 返回读取结果 0 失败  1 成功
@Info		:	连续读取2次触摸屏控制器IC,且两次的偏差不能超过ERR_RANGE误差范围
					,则认为读读正确，否则读取失败
.............................................................................*/
u8 TP_Read_XY2(u16 *x, u16 *y)
{
	u16 x1,y1;
	u16 x2, y2;
	u8 flag;
	flag = TP_Read_XY(&x1, &y1);
	if(flag == 0)
	{
		return 0;
	}
	flag = TP_Read_XY(&x2, &y2);
	if(flag == 0)
	{
		return 0;
	}
	if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
		&&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
	{
		*x = (x1+x2)/2;
		*y = (y1+y2)/2;
		return 1;
	}else
	{
		return 0;
	}
}
/*.............................................................................
@Name		:	TP_Draw_Touch_Point
@Param	:	u16 x	x坐标
					u16 y y坐标
					u16 color 颜色
@return	:	void
@Info		:	画一个触摸点,用于校准
.............................................................................*/
void TP_Draw_Touch_Point(u16 x, u16 y, u16 color)
{
	POINT_COLOR = color;
	LCD_DrawLine(x-12, y, x+13, y);						// 横线
	LCD_DrawLine(x, y-12, x, y+13);						// 竖线
	LCD_DrawPoint(x+1, y+1);
	LCD_DrawPoint(x-1, y+1);
	LCD_DrawPoint(x+1, y-1);
	LCD_DrawPoint(x-1, y-1);
	LCD_DrawCircle(x, y, 6);									//画中心圆
}
/*.............................................................................
@Name		:	TP_Draw_Touch_Point
@Param	:	u16 x	x坐标
					u16 y y坐标
					u16 color 颜色
@return	:	void
@Info		:	画一个大的点 (2*2)
.............................................................................*/
void TP_Draw_Big_Point(u16 x, u16 y, u16 color)
{
	POINT_COLOR = color;
	LCD_DrawPoint(x, y);
	LCD_DrawPoint(x+1, y);
	LCD_DrawPoint(x, y+1);
	LCD_DrawPoint(x+1, y+1);
}
/*.............................................................................
@Name		:	TP_Scan
@Param	:	u8 tp 0 屏幕坐标  1 物理坐标(校准等特殊场合采用)
@return	:	u8 触摸屏状态 0 触摸屏无触摸  1  触摸屏有触摸
@Info		:	切换触摸的模式
.............................................................................*/
u8 TP_Scan(u8 tp)
{
	if(PEN == 0)
	{
		if(tp)
		{
			TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]);
																						//读取物理坐标
		}else if(TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]))
																						//读取屏幕坐标
		{
			tp_dev.x[0] = tp_dev.xfac * tp_dev.x[0] + tp_dev.xoff;
																						//将结果转换为屏幕坐标
			tp_dev.y[0] = tp_dev.yfac * tp_dev.y[0] + tp_dev.yoff;
		}
		if((tp_dev.sta & TP_PRES_DOWN) == 0)		//触摸屏被触摸
		{
			tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES;
																						//按键按下
			tp_dev.x[4] = tp_dev.x[0];						//纪录第一次按下时的坐标
			tp_dev.y[4] = tp_dev.y[0];
		}
	}else
	{
		if(tp_dev.sta & TP_PRES_DOWN)						//触摸屏被触摸
		{
			tp_dev.sta &= ~(1<<7);								//标记按键松开
		}else
		{
			tp_dev.x[4] = 0;
			tp_dev.y[4] = 0;
			tp_dev.x[0] = 0xFFFF;
			tp_dev.y[0] = 0xFFFF;
		}
	}
	return (tp_dev.sta & TP_PRES_DOWN);				//返回当前的触摸屏状态
}
#define SAVE_ADDR_BASE 16										//保存在EEPROM里面的地址区间基址
/*.............................................................................
@Name		:	TP_Save_Adjdata
@Param	:	void
@return	:	void
@Info		:	保存校准参数
.............................................................................*/
void TP_Save_Adjdata(void)
{
	s32 temp;																	//有符号32位数据
	temp = tp_dev.xfac * 100000000;						//保存x校正因素
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE, temp, 4);
	temp = tp_dev.yfac * 100000000;						//保存y校正因素
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE+4, temp, 4);
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE+8, tp_dev.xoff, 2);		//保存x偏移量
	AT24Cxx_WriteLenByte(SAVE_ADDR_BASE+10, tp_dev.yoff, 2);	//保存y偏移量
	AT24Cxx_WriteOneByte(SAVE_ADDR_BASE+12, tp_dev.touchtype);//保存屏幕类型
	temp = 0x0A;															//标记已经校准过了
	AT24Cxx_WriteOneByte(SAVE_ADDR_BASE+13, temp);
}
/*.............................................................................
@Name		:	TP_Get_Adjdata
@Param	:	void
@return	:	u8 EEPROM中储存的数据
@Info		:	得到保存在EEPROM中的校准值
.............................................................................*/
u8 TP_Get_Adjdata(void)
{
	s32 tempfac;
	tempfac = AT24Cxx_ReadOneByte(SAVE_ADDR_BASE+13);
	if(tempfac == 0x0A)												//触摸屏已经校准过了
	{
		tempfac = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE, 4);
		tp_dev.xfac = (float) tempfac /100000000;//得到x校准参数
		tempfac = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE+4, 4);
		tp_dev.yfac = (float) tempfac /100000000;//得到y校准参数
		tp_dev.xoff = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE+8, 2);		//得到x偏移量
		tp_dev.yoff = AT24Cxx_ReadLenByte(SAVE_ADDR_BASE+10, 2);	//得到y偏移量
		tp_dev.touchtype = AT24Cxx_ReadOneByte(SAVE_ADDR_BASE+12);//读取触摸屏类型标记
		if(tp_dev.touchtype)										//X、Y方向与屏幕相反
		{
			CMD_RDX = 0x90;
			CMD_RDY = 0xD0;
		}else
		{
			CMD_RDX = 0xD0;
			CMD_RDY = 0x90;
		}
		return 1;
	}
	return 0;
}
const u8 TP_REMIND_MSG_TBL[] = "点十字，直至结束";//提示字符串
/*.............................................................................
@Name		:	TP_Adj_Info_Show
@param	: u16 x0
					u16 y0
					u16 x1
					u16 y1
					u16 x2
					u16 y2
					u16 x3
					u16 y3
					u16 fac
@return	:	void
@Info		:	提示校准结果
.............................................................................*/
void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac)
{
	POINT_COLOR = RED;
	Show_String08_NotBK(40, 160, "x1:");
	Show_String08_NotBK(40+80, 160, "y1:");
	Show_String08_NotBK(40, 180, "x2:");
	Show_String08_NotBK(40+80, 180, "y2:");
	Show_String08_NotBK(40, 200, "x3:");
	Show_String08_NotBK(40+80, 200, "y3:");
	Show_String08_NotBK(40, 220, "x4:");
	Show_String08_NotBK(40+80, 220, "y4:");
	Show_String08_NotBK(40, 240, "fac:");
	Show_Num(40+48, 160, x0);
	Show_Num(40+80+48, 160, y0);
	Show_Num(40+48, 180, x1);
	Show_Num(40+80+48, 180, y1);
	Show_Num(40+48, 200, x2);
	Show_Num(40+80+48, 200, y2);
	Show_Num(40+48, 220, x3);
	Show_Num(40+80+48, 220, y3);
	Show_Num(40+64, 240, fac);
}
/*.............................................................................
@Name		:	TP_Adjust
@Param	:	void
@return	:	void
@Info		:	触摸屏校准代码
.............................................................................*/
void TP_Adjust(void)
{
	u16 pos_temp[4][2];												//坐标缓存值
	u8 cnt = 0;
	u16 d1, d2;
	u32 temp1, temp2;
	float fac;
	u16 outtime = 0;
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	LCD_Clear(WHITE);													//清屏
	POINT_COLOR = RED;
	LCD_Clear(WHITE);
	Show_HZ16_String_NotBK(5,40,(u8*)TP_REMIND_MSG_TBL);
																						//显示提示信息
	TP_Draw_Touch_Point(20, 20, RED);
	tp_dev.sta = 0;														//消除触发信号
	tp_dev.xfac = 0;													//xfac用来标记是否校准过，所以校准之前必须清除，以免错误
	while(1)
	{
		tp_dev.scan(1);													//扫描物理坐标
		if((tp_dev.sta & 0xC0) == TP_CATH_PRES)	//按键按下了一次，此时按键松开了
		{
			outtime = 0;
			tp_dev.sta &= ~(1<<6);								//标记按键已经被处理过了
			pos_temp[cnt][0] = tp_dev.x[0];
			pos_temp[cnt][1] = tp_dev.y[0];
			cnt++;
			switch(cnt)
			{
				case 1:
				{
					TP_Draw_Touch_Point(20, 20, WHITE);//清除点1
					TP_Draw_Touch_Point(lcddev.width-20, 20, RED);
																						//画点2
					break;
				}
				case 2:
				{
 					TP_Draw_Touch_Point(lcddev.width-20,20,WHITE);
																						//清除点2
					TP_Draw_Touch_Point(20,lcddev.height-20,RED);	
																						//画点3
					break;
				}
				case 3:
				{
 					TP_Draw_Touch_Point(20,lcddev.height-20,WHITE);			
																						//清除点3
 					TP_Draw_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	
																						//画点4
					break;				
				}
				case 4:
				{
					temp1 = abs(pos_temp[0][0] - pos_temp[1][0]);
																						//x1 - x2
					temp2 = abs(pos_temp[0][1] - pos_temp[1][1]);
																						//y1 - y2
					temp1 *= temp1;
					temp2 *= temp2;
					d1 = sqrt(temp1 + temp2);					//得到1、2的距离
					temp1 = abs(pos_temp[2][0] - pos_temp[3][0]);
																						//x3 - x4
					temp2 = abs(pos_temp[2][1] - pos_temp[3][1]);
																						//y3 - y4
					temp1 *= temp1;
					temp2 *= temp2;
					d2 = sqrt(temp1 + temp2);					//得到3、4的距离			
					fac = (float) d1/d2;
					if(fac<0.95 || fac>1.05 || d1 == 0 || d2 == 0)
																						//校准的点不合格
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//清除点4
						TP_Draw_Touch_Point(20, 20, RED);//画点1
						TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
						pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac*100);
																						//显示校准的结果
						continue;
					}
					temp1 = abs(pos_temp[0][0] - pos_temp[2][0]);
																						//x1 - x3
					temp2 = abs(pos_temp[0][1] - pos_temp[2][1]);
																						//y1 - y3
					temp1 *= temp1;
					temp2 *= temp2;
					d1 = sqrt(temp1+temp2);						//得到1、3的距离
					temp1 = abs(pos_temp[1][0] - pos_temp[3][0]);
																						//x2 - x4
					temp2 = abs(pos_temp[1][1] - pos_temp[3][1]);
																						//y2 - y4
					temp1 *= temp1;
					temp2 *= temp2;
					d2 = sqrt(temp1+temp2);						//得到2、4的距离
					fac = (float)d1 / d2;
					if(fac<0.95 || fac>1.05)					//校准的点不合格
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//清除点4
						TP_Draw_Touch_Point(20, 20, RED);//画点1
						TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
						pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac*100);
																						//显示校准的结果
						continue;
					}
					temp1 = abs(pos_temp[1][0] - pos_temp[2][0]);
																						//x1 - x3
					temp2 = abs(pos_temp[1][1] - pos_temp[2][1]);
																						//y1 - y3
					temp1 *= temp1;
					temp2 *= temp2;
					d1 = sqrt(temp1+temp2);						//得到1、4的距离					
					temp1 = abs(pos_temp[0][0] - pos_temp[3][0]);
																						//x1 - x3
					temp2 = abs(pos_temp[0][1] - pos_temp[3][1]);
																						//y1 - y3
					temp1 *= temp1;
					temp2 *= temp2;
					d2 = sqrt(temp1+temp2);						//得到2、3的距离			
					fac = (float)d1 / d2;
					if(fac<0.95 || fac>1.05)					//校准的点不合格
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//清除点4
						TP_Draw_Touch_Point(20, 20, RED);//画点1
						TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
						pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac*100);
																						//显示校准的结果
						continue;
					}
					tp_dev.xfac = (float) (lcddev.width-40) / (pos_temp[1][0] - pos_temp[0][0]);
																						//得到xfac
					tp_dev.xoff = (lcddev.width-tp_dev.xfac*(pos_temp[1][0] + pos_temp[0][0])) / 2;
																						//得到xoff
					tp_dev.yfac = (float) (lcddev.height-40) / (pos_temp[2][1] - pos_temp[0][1]);
																						//得到yfac
					tp_dev.yoff = (lcddev.height-tp_dev.yfac*(pos_temp[2][1] + pos_temp[0][1])) / 2;
																						//得到yoff					
					if(abs(tp_dev.xfac)>2 || abs(tp_dev.yfac)>2)
																						//触摸屏和预设的相反
					{
						cnt = 0;
						TP_Draw_Touch_Point(lcddev.width-20, lcddev.height-20, WHITE);
																						//清除点4
						TP_Draw_Touch_Point(20, 20, RED);//清除点1
						Show_String08_NotBK(40, 26, "TP Need ReadJust !");
						tp_dev.touchtype = ! tp_dev.touchtype;
																						//修改触摸屏类型
						if(tp_dev.touchtype)						//X，Y方向与屏幕相反
						{
							CMD_RDX = 0x90;
							CMD_RDY = 0xD0;
						}else														//X,Y方向与屏幕相同
						{
							CMD_RDX = 0xD0;
							CMD_RDY = 0x90;
						}
						continue;
					}
					POINT_COLOR = BLUE;
					LCD_Clear(WHITE);									//清屏
					Show_String08_NotBK(35, 110, "Touch Screen Adjust OK!");
																						//校正完成
					delay_ms(1000);
					TP_Save_Adjdata();								//保存校准参数
					LCD_Clear(WHITE);									//清屏
					return;														//校正完成
				}
			}
		}
		delay_ms(10);
		outtime ++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();											//得到保存在EEPROM中的校准值
			break;
		}
	}
}
/*.............................................................................
@Name		:	TP_Init
@Param	:	void
@return	:	u8 返回屏幕校准的结果 0 没有进行校准  1 进行过校准
@Info		:	初始化触摸屏
.............................................................................*/
u8 TP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	
																						//使能PB、PD端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB14端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
																						// PB12、PB13、PB15端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//上拉
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//D13端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	TP_Read_XY(&tp_dev.x[0], &tp_dev.y[0]);//第一次读取初始化
	if(TP_Get_Adjdata())											//已经校准
	{
		return 0;
	}else
	{
		LCD_Clear(WHITE);												//清屏
		TP_Adjust();														//屏幕校准
		TP_Save_Adjdata();											//保存校准参数
	}
	TP_Get_Adjdata();
	return 1;	
}
/********************************************************************
*name:Test_TP
*description:初始化屏幕测试
*param:void
*return:void
********************************************************************/
void Test_Init(void)
{
	tp_dev.init();
	delay_ms(1500);
	LCD_Clear(WHITE);
	tp_dev.scan(0);							//屏幕扫描
}
/********************************************************************
*name:Test_Touch
*description:测试触摸屏
*param:void
*return:void
********************************************************************/
void Test_Touch(void)
{
	u8 key;
	while(1)
	{
		key = Get_Key();
		tp_dev.scan(0);						//扫描触摸屏
		if(tp_dev.sta & TP_PRES_DOWN)//触摸屏被触摸
		{
			if(tp_dev.x[0]<lcddev.width && tp_dev.y[0]<lcddev.height)
			{
				if(tp_dev.x[0]>(lcddev.width-24) && tp_dev.y[0]<16)
				{
					LCD_Clear(WHITE);
					POINT_COLOR = BLUE;
					Show_String08_NotBK(40, 40, "Press KEY0 to Adjust!");
				}else
				{
					TP_Draw_Big_Point(tp_dev.x[0], tp_dev.y[0], RED);
				}
			}
		}else
		{
			delay_ms(10);
		}
		if(key == 1)								//按键按下,执行校准程序						
		{
			LCD_Clear(WHITE);				//清屏
			TP_Adjust();						//屏幕校准
			TP_Save_Adjdata();			//保存校准结果
			LCD_Clear(WHITE);
		}
	}
}
