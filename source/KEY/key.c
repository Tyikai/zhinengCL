#include "key.h"
#include "delay.h"
/*.............................................................................
@Name	:	Key_Init
@Param	:	void 
@return	:	void
@Info	:	初始化按键
.............................................................................*/
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
											//使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//配置PA12 端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//根据设定值，配置PA12 端口
	
}
/*.............................................................................
@Name	:	Get_Key
@Param	:	void 
@return	:	void
@Info	:	获取按键值 1 有按键  0  无按键
.............................................................................*/
u8 Get_Key(void)
{
	if(K_Adj==0)							//如果按键按下
	{
		delay_ms(30);						//延时消除抖动
		if(K_Adj==0)
		{
			return 1;
		}else
		{
			return 0;
		}
	}
}

