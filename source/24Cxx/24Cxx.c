#include "24Cxx.h"
#include "My_IIC.h"
#include "delay.h"
/*.............................................................................
@Name		:	AT24Cxx_Init
@Param	:	void 
@return	:	void
@Info		:	³õÊ¼»¯ EEPROMÐ¾Æ¬ AT24C02
.............................................................................*/
void AT24Cxx_Init(void)
{
	IIC_Init();
}
/*.............................................................................
@Name		:	AT24Cxx_ReadOneByte
@Param	:	u16 ReadAddr ¿ªÊ¼¶ÁÈ¡µÄµØÖ· 
@return	:	 u8 ¶ÁÈ¡µ½µÄÊý¾Ý
@Info		:	ÔÚAT24CxxÖ¸¶¨µØÖ·¶ÁÈ¡Ò»¸öÊý¾Ý
.............................................................................*/
u8 AT24Cxx_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);										//·¢ËÍÐ´Ö¸Áî,Ç°4bit¹Ì¶¨Îª1010 ½ô½Ó3bitÎª A2 A1 A0 Ó²¼þ¾ö¶¨,
																						//(ÏµÍ³×î¶à¿É½Ó8¸öÉè±¸2^3=8) ×îºó1bit¾ö¶¨¶ÁÐ´ 1 ¶Á 0 Ð´
																						//0xA0 = 1010 000 0Ð´
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);							//·¢ËÍ¸ßµØÖ·
	}else
	{
		IIC_Send_Byte(0xA0+((ReadAddr/256)<<1));//»ñÈ¡µØÖ·¸ß8Î»
		                                        //C01¡¢C02 1010 A2 A1 A0 R/W
		                                        //C04      1010 A2 A1 a8 R/W ----a8µØÖ·µÄµÚ9Î»
																						//C08			1010 A2 a9 a8 R/W	----a9µØÖ·µÄµÚ10Î»
																						//C16      1010 a10 a9 a8 R/W --- a10µØÖ·µÄµÚ11Î»
																						//µ±IICÉè±¸ÎªC16Ê±½ü¿ÉÒÔ¹ÒÔØ1¸ö£¬ÒòÎªÎÞµØÖ·¿ÉÐÞ¸Ä
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr%256); 							//·¢ËÍµØÖ·µÍ8Î»
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0xA1);											//½øÈë½ÓÊÕÄ£Ê½
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();																//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ
	return temp;
}
/*.............................................................................
@Name		:	AT24Cxx_WriteOneByte
@Param	:	u16 WriteAddr Ð´ÈëÊý¾ÝµÄÄ¿µÄµØÖ·
					8 data ´ýÐ´ÈëµÄÊý¾Ý
@return	:	void
@Info		:	ÔÚAT24CxxÖ¸¶¨µÄµØÖ·Ð´ÈëÒ»¸öÊý¾Ý
.............................................................................*/
void AT24Cxx_WriteOneByte(u16 WriteAddr, u8 data)
{
	IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);										//·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);						//·¢ËÍ¸ßµØÖ·
	}else
	{
		IIC_Send_Byte(0xA0+((WriteAddr/256)<<1));//·¢ËÍÆ÷¼þ¸ßµØÖ·
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr%256);							//·¢ËÍµÍµØÖ·
	IIC_Wait_Ack();
	IIC_Send_Byte(data);											//·¢ËÍ×Ö½Ú
	IIC_Wait_Ack();
	IIC_Stop();																//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ
	delay_ms(10);
}
/*.............................................................................
@Name		:	AT24Cxx_WriteLenByte
@Param	:	u16 WriteAddr Ð´ÈëÊý¾ÝµÄÄ¿µÄµØÖ·
					8 data ´ýÐ´ÈëµÄÊý¾Ý
					u8 len Ð´ÈëÊý¾ÝµÄ³¤¶È
@return	:	void
@Info		:	ÔÚAT24CxxÀïÃæÖ¸¶¨µÄµØÖ·¿ªÊ¼Ð´Èë³¤¶ÈÎªlenµÄÊý¾Ý
.............................................................................*/
void AT24Cxx_WriteLenByte(u16 WriteAddr, u32 data, u8 len)
{
	u8 t;
	for(t=0;t<len;t++)
	{
		AT24Cxx_WriteOneByte(WriteAddr++, (data>>(8*t))&0xFF);
	}
}
/*.............................................................................
@Name		:	AT24Cxx_ReadLenByte
@Param	:	u16 WriteAddr Ð´ÈëÊý¾ÝµÄÄ¿µÄµØÖ·
					u8 len Êý¾Ý³¤¶È 1,2,4
@return	:	u32 ¶ÁÈ¡µ½µÄÊý¾Ý
@Info		:	´ÓAT24CxxÐ¾Æ¬ÖÐ¶ÁÈ¡Ö¸¶¨³¤¶ÈµÄÊý¾Ý
.............................................................................*/
u32 AT24Cxx_ReadLenByte(u16 ReadAddr, u8 len)
{
	u8 t;
	u32 temp = 0;
	for(t=0;t<len;t++)
	{
		temp <<= 8;
		temp += AT24Cxx_ReadOneByte(ReadAddr+len-t-1);
	}
	return temp;
}
/*.............................................................................
@Name		:	AT24Cxx_Check
@Param	:	void
@return	:	u8 ·µ»Ø¼ì²é½á¹û 0 ³É¹¦  1 Ê§°Ü 
@Info		:	¼ì²éAT24CxxÊÇ·ñ³É¹¦
.............................................................................*/
u8 AT24Cxx_Check(void)
{
	u8 temp;
	temp = AT24Cxx_ReadOneByte(255);					//±ÜÃâÃ¿´Î¿ª»ú¶¼Ð´AT24Cxx
	if(temp == 0x55)
	{
		return 0;																//ÅÅ³ýµÚÒ»´Î³õÊ¼»¯µÄÇé¿ö
	}else
	{
		AT24Cxx_WriteOneByte(255, 0x55);
		temp = AT24Cxx_ReadOneByte(255);
		if(temp == 0x55)
		{
			return 0;
		}
	}
	return 1;
}
/*.............................................................................
@Name		:	AT24Cxx_Read
@Param	:	u16 ReadAddr Ö¸¶¨¶ÁÈ¡µÄµØÖ·
					u8 *pBuffer ´æ´¢Êý¾ÝÊý×éµÄÊ×µØÖ·
					u16 num ¶ÁÈ¡Êý¾ÝµÄ¸öÊý
@return	:	void
@Info		:	´ÓAT24CxxÖ¸¶¨µÄµØÖ·¶ÁÈ¡Ö¸¶¨¸öÊýµÄÊý¾Ý
.............................................................................*/
void AT24Cxx_Read(u16 ReadAddr, u8 *pBuffer, u16 num)
{
	while(num --)
	{
		*pBuffer ++ =AT24Cxx_ReadOneByte(ReadAddr ++);
	}
}
/*.............................................................................
@Name		:	AT24Cxx_Write
@Param	:	u16 WriteAddr Ð´ÈëÊý¾ÝµÄµØÖ·
					u8 *pBufferÐ´ÈëÊý¾ÝµÄÊý×éÊ×µØÖ·
					u16 num Ð´ÈëÊý¾ÝµÄ³¤¶
@return	:	void
@Info		:	ÔÚAT24CxxÖÐÖ¸¶¨µÄÎ»ÖÃÐ´ÈëÖ¸¶¨³¤¶ÈµÄÊý¾Ý
.............................................................................*/
void AT24Cxx_Write(u16 WriteAddr, u8 *pBuffer, u16 num)
{
	while(num--)
	{
		AT24Cxx_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr ++;
		pBuffer ++;
	}
}

