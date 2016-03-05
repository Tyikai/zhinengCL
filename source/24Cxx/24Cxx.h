#ifndef AT24CXX_H_
#define AT24CXX_H_
#include "sys.h"
#define AT24C01			127
#define AT24C02			255
#define AT24C04			511
#define AT24C08			1023
#define AT24C16			2047
#define AT24C32			4095
#define AT24C64			8191
#define AT24C128		16383
#define AT24C256		32767
#define EE_TYPE AT24C02											//Ä¬ÈÏEEPROmÐ¾Æ¬
void AT24Cxx_Init(void);
u8 AT24Cxx_ReadOneByte(u16 ReadAddr);
void AT24Cxx_WriteOneByte(u16 WriteAddr, u8 data);
void AT24Cxx_WriteLenByte(u16 WriteAddr, u32 data, u8 len);
u32 AT24Cxx_ReadLenByte(u16 ReadAddr, u8 len);
u8 AT24Cxx_Check(void);
void AT24Cxx_Read(u16 ReadAddr, u8 *pBuffer, u16 num);
void AT24Cxx_Write(u16 WriteAddr, u8 *pBuffer, u16 num);
#endif

