#include "rtc.h"
#include "sys.h"
#include "delay.h"
_calendar_obj calendar;
/*.............................................................................
							ʮ�����£�ÿ����ӵ�е�����
.............................................................................*/	
const u8 mon_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 
/*.............................................................................
							���� ,���������ݱ�
.............................................................................*/
const u8 table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};	
/*.............................................................................
@Name	:	RTC_NVIC_Config
@Param	:	void
@return	:	void
@Info	:	RTC��NVIC����
.............................................................................*/
static void RTC_NVIC_Config(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
											//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
											//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
											//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);			//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}
/*.............................................................................
@Name	:	RTC_Init
@Param	:	void
@return	:	void
@Info	:	��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
.............................................................................*/
u8 RTC_Init(void)
{
											//����ǲ��ǵ�һ������ʱ��
	u8 temp=0;
 
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x3050)		
											//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{	 			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	
											//ʹ��PWR��BKP����ʱ��   
		PWR_BackupAccessCmd(ENABLE);		//ʹ�ܺ󱸼Ĵ������� 
		BKP_DeInit();						//��λ�������� 	
		RCC_LSEConfig(RCC_LSE_ON);			//�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	
											//���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			delay_ms(10);
		}
		if(temp>=250)return 1;				//��ʼ��ʱ��ʧ��,����������	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);				//��RTCʱ��  
		RTC_WaitForLastTask();				//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();				//�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
		RTC_WaitForLastTask();				//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();				/// ��������	
		RTC_SetPrescaler(32767); 			//����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();				//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_Set(2016,3,1,12,0,0);  			//����ʱ��	
		RTC_ExitConfigMode(); 				//�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, 0X3050);//��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	else									//ϵͳ������ʱ
	{
		RTC_WaitForSynchro();				//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
		RTC_WaitForLastTask();				//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}
	RTC_NVIC_Config();						//RCT�жϷ�������		    				     
	RTC_Get();								//����ʱ��	
	return 0; 								//ok
}
/*.............................................................................
@Name	:	RTC_IRQHandler
@Param	:	void
@return	:	void
@Info	:	RTCʱ���жϣ�һ����һ��
.............................................................................*/
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
	{							
		RTC_Get();							//����ʱ��   
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)	//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	//�������ж�	  	   
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);//�������ж�
	RTC_WaitForLastTask();	  	    						 	   	 
}
/*.............................................................................
@Name	:	RTC_Set
@Param	: 	u16 syear ���
			u8 smon �·�
			u8 sday ����
			u8 hour Сʱ
			u8 min  ����
			u8 sec  ����
@return	:	u8 ���ý�� 0�ɹ��� 1ʧ��
@Info	:	����ʱ�ӣ��Զ���
.............................................................................*/
u8  RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
	u16 t;
	u32 seccount = 0;
	if(syear <1970 || syear > 2099)
	{
		return 1;
	}
	for(t=1970;t<syear;t++)
	{
		if(Is_Leap_Year(t))
		{
			seccount += 31622400;			//�����������
		}else
		{
			seccount += 31536000;			//ƽ���������
		}
	}
	smon -= 1;								//��0��ʼ���
	for(t=0;t<smon;t++)
	{
		seccount += (u32) mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear) && t == 1)
		{
			seccount += 86400;				//���꣬���·ݶ�һ��
		}
	}
	seccount += (u32) (sday-1)*86400;		//����ӵ�е�����
	seccount += (u32) hour*3600;			//Сʱ���е�����
	seccount += (u32) min*60;				//����ӵ�е�����
	seccount += sec;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	
											//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);			//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(seccount);				//����RTC��������ֵ

	RTC_WaitForLastTask();					//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;	 	
}