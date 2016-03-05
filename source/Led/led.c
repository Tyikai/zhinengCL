#include "led.h"
#include "sys.h"
/*.............................................................................
@Name		:	Led_Init
@Param	:	void
@return	:	void
@Info		:	��ʼ��LED PB0
.............................................................................*/
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD,ENABLE);
																						//ʹ��GPIOBʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//����PB5 �˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����I/O�ڵ��ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);		//�����趨ֵ������PB5 �˿�
}

