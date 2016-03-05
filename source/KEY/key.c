#include "key.h"
#include "delay.h"
/*.............................................................................
@Name	:	Key_Init
@Param	:	void 
@return	:	void
@Info	:	��ʼ������
.............................................................................*/
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
											//ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//����PA12 �˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//�����趨ֵ������PA12 �˿�
	
}
/*.............................................................................
@Name	:	Get_Key
@Param	:	void 
@return	:	void
@Info	:	��ȡ����ֵ 1 �а���  0  �ް���
.............................................................................*/
u8 Get_Key(void)
{
	if(K_Adj==0)							//�����������
	{
		delay_ms(30);						//��ʱ��������
		if(K_Adj==0)
		{
			return 1;
		}else
		{
			return 0;
		}
	}
}

