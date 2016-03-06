#ifndef CHINESEFONT_H_
#define CHINESEFONT_H_
typedef struct
{
	unsigned char Index[2];			//保存汉字区号和位号
	unsigned char Marks[32];							//保存显示的十六进制数
}type_GB16;
/*.............................................................................
@字体：楷体
@字宽：16
@字高：16
@修改点阵：无
@说明：1、阴码
			 2、顺向（高位在前）
			 3、逐行式
.............................................................................*/
const type_GB16 FONT16[]={
"智",
0x00,0x00,0x04,0x00,0x04,0x00,0x09,0x80,0x0E,0x7E,0x35,0xC4,0x1E,0x48,0x25,0x30,
0x08,0x00,0x13,0xF0,0x22,0x20,0x03,0xE0,0x02,0x20,0x02,0xE0,0x03,0x20,0x00,0x20,
"能",
0x00,0x00,0x00,0x80,0x0C,0x80,0x08,0x90,0x12,0xE0,0x22,0x80,0x3C,0x88,0x04,0x70,
0x1A,0x80,0x1C,0x90,0x24,0xE0,0x1A,0x80,0x22,0x84,0x26,0x4C,0x04,0x30,0x00,0x00,
"光",
0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x10,0x01,0x30,0x05,0x40,0x01,0x00,0x01,0xF8,
0x1E,0x80,0x02,0x80,0x04,0x80,0x0C,0x82,0x08,0x82,0x10,0x46,0x60,0x7C,0x00,0x00,
"感",
0x00,0x00,0x00,0x80,0x00,0x90,0x00,0xE0,0x0F,0x80,0x09,0x50,0x0E,0x50,0x13,0xA0,
0x1D,0x20,0x17,0x50,0x21,0x8C,0x40,0x88,0x14,0x24,0x13,0x10,0x00,0xF0,0x00,0x00,
"窗",
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0xFC,0x1F,0x0E,0x26,0x60,0x09,0x00,0x02,0x00,
0x0F,0xF0,0x09,0x10,0x0A,0xD0,0x0D,0x90,0x0A,0x90,0x0C,0xF0,0x0B,0x30,0x00,0x00,
"帘",
0x00,0x00,0x00,0x00,0x01,0x88,0x11,0xFC,0x1E,0x08,0x32,0x30,0x25,0x00,0x18,0xA0,
0x0F,0xD0,0x04,0x90,0x08,0x90,0x08,0xF0,0x00,0xA0,0x01,0x00,0x01,0x00,0x00,0x00,
"点",
0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x10,0x01,0xE0,0x01,0x00,0x01,0x00,0x0F,0xF0,
0x04,0x20,0x04,0x20,0x07,0xC0,0x00,0x08,0x22,0x4C,0x22,0x46,0x20,0x00,0x00,0x00,
"十",
0x00,0x00,0x01,0x00,0x01,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x03,0xFC,0x3C,0x80,
0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
"字",
0x00,0x00,0x01,0x00,0x00,0x80,0x00,0xF8,0x0F,0x1C,0x10,0xE0,0x17,0x40,0x00,0x80,
0x00,0xBC,0x7F,0xC0,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x01,0x80,0x00,0x00,
"，",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,
"直",
0x00,0x00,0x01,0x80,0x01,0x00,0x01,0x70,0x0F,0x80,0x02,0x00,0x07,0xE0,0x04,0x20,
0x07,0x20,0x04,0x20,0x07,0x20,0x07,0xC0,0x04,0x40,0x07,0xFE,0x38,0x00,0x00,0x00,
"至",
0x00,0x00,0x00,0x00,0x00,0x70,0x07,0x80,0x01,0x00,0x02,0x20,0x04,0x30,0x0F,0xD0,
0x01,0x80,0x01,0x00,0x07,0xE0,0x01,0x00,0x01,0x08,0x7F,0xF6,0x00,0x00,0x00,0x00,
"结",
0x00,0x00,0x00,0x00,0x04,0x20,0x08,0x20,0x08,0x20,0x12,0x2E,0x25,0xF0,0x38,0x20,
0x12,0x3C,0x3C,0x40,0x02,0x3C,0x0C,0xC4,0x70,0x88,0x00,0x70,0x00,0x00,0x00,0x00,
"束",
0x00,0x00,0x01,0x00,0x01,0x00,0x01,0xC0,0x07,0x00,0x01,0xF0,0x1F,0x20,0x09,0x40,
0x0F,0x80,0x0B,0x00,0x05,0xC0,0x09,0x30,0x11,0x1C,0x61,0x00,0x01,0x00,0x01,0x00,
"强",
0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0xFC,0x04,0x88,0x04,0xF0,0x38,0x20,0x20,0x2C,
0x21,0xF4,0x3D,0x28,0x08,0xF0,0x08,0x28,0x08,0x3C,0x2B,0xC4,0x18,0x00,0x00,0x00,
"自",
0x00,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x60,0x0D,0xB0,0x08,0x10,0x08,0x10,
0x07,0x10,0x08,0x10,0x07,0x90,0x08,0x10,0x08,0xD0,0x07,0x30,0x00,0x20,0x00,0x00,
"手",
0x00,0x00,0x00,0x00,0x00,0xE0,0x03,0x00,0x01,0x00,0x00,0xE0,0x03,0x80,0x00,0x80,
0x03,0xFE,0x3C,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x01,0x80,0x03,0x00,0x00,0x00,
"开",
0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x06,0x40,0x02,0x40,0x02,0x4C,0x7F,0xF0,
0x04,0x40,0x04,0x40,0x04,0x40,0x08,0x40,0x08,0x40,0x10,0x40,0x00,0x00,0x00,0x00,
"关",
0x00,0x00,0x00,0x20,0x00,0x20,0x06,0x40,0x02,0x40,0x00,0xE0,0x07,0x00,0x01,0x00,
0x03,0xFC,0x1D,0x00,0x02,0x80,0x02,0x40,0x04,0x20,0x08,0x38,0x10,0x1E,0x00,0x00,
"年",
0x00,0x00,0x02,0x00,0x02,0x00,0x05,0xE0,0x06,0x80,0x18,0x90,0x07,0xE0,0x04,0x80,
0x04,0xFE,0x3F,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,
"月",
0x00,0x00,0x00,0x00,0x00,0x70,0x07,0xB0,0x02,0x20,0x02,0x20,0x03,0xA0,0x04,0x20,
0x04,0xA0,0x07,0x20,0x04,0x20,0x08,0x20,0x08,0x20,0x10,0x60,0x20,0x20,0x00,0x00,
"日",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x30,0x08,0x20,0x04,0x20,0x05,0xA0,
0x06,0x20,0x08,0x20,0x08,0x20,0x09,0xA0,0x06,0x60,0x00,0x20,0x00,0x00,0x00,0x00,
"时",
0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x20,0x3C,0x20,0x24,0x20,0x27,0xFC,0x24,0x20,
0x3D,0x20,0x24,0xA0,0x24,0x10,0x3C,0x10,0x24,0x10,0x00,0x60,0x00,0x20,0x00,0x00,
"分",
0x00,0x00,0x03,0x00,0x00,0x80,0x04,0x80,0x04,0x40,0x0C,0x20,0x08,0x18,0x10,0x1E,
0x27,0xE0,0x1A,0x20,0x04,0x40,0x04,0x40,0x08,0x40,0x11,0xC0,0x01,0x80,0x00,0x00,
"秒",
0x00,0x00,0x00,0x00,0x02,0x20,0x06,0x20,0x18,0x20,0x04,0x26,0x1C,0xA0,0x28,0xA8,
0x0F,0x28,0x18,0x10,0x28,0x10,0x48,0x20,0x08,0x40,0x00,0x80,0x01,0x00,0x00,0x00,
};
typedef struct
{
	unsigned char Index[2];			//保存汉字区号和位号
	unsigned char Marks[72];		//保存显示的十六进制数
}type_GB24;
/********************************************************************
*字体：华文行楷
*字宽：20
*字高：28
*修改点阵：左 0
           上 1
           右 4
           下 -5
*说明：1、阴码
			 2、顺向（高位在前）
			 3、逐行式
*********************************************************************/
const type_GB24 FONT24[]={
"光",
0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,0xCC,0x00,0x00,0xC4,0x00,0x00,
0x64,0x00,0x80,0x24,0x00,0x80,0x15,0x00,0x00,0x04,0x00,0x00,0xC4,0x00,0x00,0xFC,
0x01,0xF0,0x0B,0x00,0x60,0x1A,0x00,0x00,0x1B,0x00,0x00,0x09,0x00,0x00,0x09,0x00,
0x80,0x09,0x00,0x80,0x08,0x00,0x40,0x08,0x04,0x60,0x08,0x04,0x20,0x08,0x0C,0x10,
0x30,0x0E,0x08,0xF0,0x07,0x00,0x00,0x00,
"强",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x01,0xCF,0xE0,0x02,0xC8,0x40,0x00,
0x88,0x40,0x00,0x88,0xC0,0x00,0x87,0x00,0x01,0x83,0x00,0x06,0x03,0x00,0x04,0x02,
0x60,0x04,0x07,0xB0,0x04,0x1A,0x20,0x0F,0x92,0x20,0x0C,0x92,0x40,0x08,0x8F,0x80,
0x00,0x82,0x00,0x00,0x82,0x40,0x00,0x82,0x60,0x00,0x83,0xA0,0x01,0xBE,0x30,0x07,
0x30,0x30,0x03,0x00,0x10,0x00,0x00,0x00,
"：",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,
0xE0,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
typedef struct
{
	unsigned char Index[2];			//保存汉字区号和位号
	unsigned char Marks[288];		//保存显示的十六进制数
}type_GB48;
/********************************************************************
*字体：华文行楷
*字宽：32
*字高：32
*修改点阵：左 0
           上 -1
           右 0
           下 1
*说明：1、阴码
			 2、顺向（高位在前）
			 3、逐行式
*********************************************************************/
const type_GB48 FONT48[]={
"智",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x01,
0xE0,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,
0x00,0x03,0x81,0x80,0x00,0x00,0x00,0x03,0x0F,0xC0,0x00,0x00,0x00,0x06,0x7E,0x00,
0x07,0x80,0x00,0x0F,0xF0,0x18,0x7F,0xE0,0x00,0x0C,0x70,0x1F,0xE1,0xF0,0x00,0x18,
0x70,0x0C,0x01,0xE0,0x00,0x30,0x70,0x0E,0x01,0xC0,0x00,0x40,0x63,0xE6,0x03,0x80,
0x00,0x00,0x7F,0x86,0x03,0x80,0x00,0x03,0xF8,0x06,0x03,0x00,0x00,0x3F,0xE0,0x06,
0x07,0x00,0x0F,0xF8,0xC0,0x07,0xFF,0x80,0x07,0x81,0xCF,0x07,0x80,0x00,0x00,0x01,
0xC7,0x82,0x00,0x00,0x00,0x01,0x87,0x80,0x00,0x00,0x00,0x03,0x81,0x80,0x00,0x00,
0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x80,0x00,0x00,0x0C,0x00,0xFF,
0xE0,0x00,0x00,0x18,0x7F,0x03,0xE0,0x00,0x00,0x30,0x70,0x01,0xE0,0x00,0x00,0x40,
0x70,0x01,0xE0,0x00,0x01,0x80,0x30,0x01,0xC0,0x00,0x00,0x00,0x30,0x01,0xC0,0x00,
0x00,0x00,0x30,0xF9,0xC0,0x00,0x00,0x00,0x3F,0xE1,0xC0,0x00,0x00,0x00,0x30,0x01,
0xC0,0x00,0x00,0x00,0x30,0x01,0xC0,0x00,0x00,0x00,0x30,0x01,0xC0,0x00,0x00,0x00,
0x30,0x01,0xC0,0x00,0x00,0x00,0x30,0x01,0xC0,0x00,0x00,0x00,0x31,0xF9,0xC0,0x00,
0x00,0x00,0x3F,0xF7,0xC0,0x00,0x00,0x00,0x30,0x03,0xC0,0x00,0x00,0x00,0x30,0x03,
0xC0,0x00,0x00,0x00,0x20,0x03,0x80,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
"能",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x07,
0x00,0xF0,0x00,0x00,0x00,0x07,0x80,0x70,0x00,0x00,0x00,0x07,0x80,0x70,0x00,0x00,
0x00,0x07,0x80,0x70,0x00,0x00,0x00,0x0F,0x00,0x60,0x60,0x00,0x00,0x0E,0x00,0x60,
0xF0,0x00,0x00,0x1C,0x00,0x61,0xF0,0x00,0x00,0x18,0x60,0x63,0xC0,0x00,0x00,0x30,
0x70,0x6E,0x00,0x00,0x00,0x30,0x38,0x78,0x00,0x00,0x00,0x60,0x3C,0x60,0x02,0x00,
0x00,0xC0,0x3C,0x60,0x02,0x00,0x01,0x87,0xDC,0x60,0x06,0x00,0x03,0xFE,0x0C,0x60,
0x06,0x00,0x03,0xF8,0x0C,0x70,0x0E,0x00,0x03,0xC0,0x00,0x3F,0xFE,0x00,0x01,0x00,
0x00,0x3F,0xFE,0x00,0x00,0x03,0xE0,0x0F,0xF8,0x00,0x00,0xFF,0xF0,0x00,0x00,0x00,
0x00,0x60,0x70,0xE0,0x00,0x00,0x00,0x60,0x70,0x70,0x00,0x00,0x00,0x60,0x60,0x70,
0x30,0x00,0x00,0x60,0x60,0x70,0x70,0x00,0x00,0x7F,0x60,0x70,0xF8,0x00,0x00,0x60,
0x60,0x61,0xF0,0x00,0x00,0x60,0x60,0x67,0x80,0x00,0x00,0x40,0x60,0x6E,0x00,0x00,
0x00,0x40,0x60,0x78,0x00,0x00,0x00,0xDE,0x60,0x60,0x00,0x00,0x00,0xFE,0x70,0x60,
0x00,0x40,0x00,0xC0,0x70,0x60,0x00,0x40,0x00,0xC0,0x70,0x60,0x00,0x40,0x01,0xC0,
0x70,0x60,0x00,0x60,0x01,0xC0,0x70,0x70,0x00,0xE0,0x03,0x86,0xF0,0x78,0x01,0xE0,
0x03,0x83,0xF0,0x3E,0x07,0xE0,0x03,0x81,0xE0,0x1F,0xFF,0xE0,0x01,0x00,0xE0,0x0F,
0xFF,0x80,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
"光",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,
0x07,0xC0,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,
0x00,0x00,0x03,0x80,0xC0,0x00,0x00,0x00,0x03,0x80,0xE0,0x00,0x00,0x00,0x03,0x81,
0xF0,0x00,0x00,0x00,0x03,0x81,0xF8,0x00,0x00,0x00,0x03,0x83,0xE0,0x00,0x00,0x03,
0x83,0x87,0x80,0x00,0x00,0x03,0xC3,0x8E,0x00,0x00,0x00,0x01,0xC3,0x9C,0x00,0x00,
0x00,0x01,0xC3,0xB0,0x00,0x00,0x00,0x00,0xC3,0x80,0x00,0x00,0x00,0x00,0x03,0x80,
0x00,0x00,0x00,0x00,0x03,0x80,0xF0,0x00,0x00,0x00,0x03,0x9F,0xFC,0x00,0x00,0x00,
0x0F,0xFF,0xFC,0x00,0x00,0x07,0xFF,0xE0,0x00,0x00,0x00,0xFF,0xF0,0xE0,0x00,0x00,
0x00,0x7E,0x38,0x70,0x00,0x00,0x00,0x00,0x3C,0x70,0x00,0x00,0x00,0x00,0x3C,0x70,
0x00,0x00,0x00,0x00,0x78,0x60,0x00,0x00,0x00,0x00,0x70,0x60,0x00,0x00,0x00,0x00,
0xE0,0x60,0x00,0x00,0x00,0x00,0xE0,0x60,0x00,0x00,0x00,0x01,0xC0,0x60,0x00,0x00,
0x00,0x03,0x80,0x60,0x00,0x10,0x00,0x07,0x80,0x60,0x00,0x10,0x00,0x07,0x00,0x60,
0x00,0x10,0x00,0x0E,0x00,0x60,0x00,0x10,0x00,0x1C,0x00,0x60,0x00,0x30,0x00,0x38,
0x00,0x60,0x00,0x38,0x00,0x70,0x00,0x70,0x00,0x78,0x00,0xE0,0x00,0x38,0x00,0xF8,
0x01,0x80,0x00,0x3F,0xFF,0xF8,0x06,0x00,0x00,0x1F,0xFF,0xF0,0x08,0x00,0x00,0x0F,
0xFF,0xC0,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
"感",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,
0x01,0xC1,0xE0,0x00,0x00,0x00,0x00,0xC1,0xF0,0x00,0x00,0x00,0x00,0xE0,0x78,0x00,
0x00,0x00,0x00,0xE0,0x38,0x00,0x00,0x00,0x00,0x67,0x80,0x00,0x00,0x00,0x00,0x7F,
0x80,0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,0x18,0xFF,0xB0,0x00,0x00,0x00,0x1F,
0xE0,0x30,0x00,0x00,0x00,0x0C,0x00,0x38,0x00,0x00,0x00,0x0C,0x00,0x18,0x20,0x00,
0x00,0x0C,0x07,0x98,0x30,0x00,0x00,0x0C,0x3F,0x9C,0x38,0x00,0x00,0x0C,0xFC,0x0C,
0x38,0x00,0x00,0x0C,0x00,0x0E,0x70,0x00,0x00,0x0C,0x00,0x06,0x60,0x00,0x00,0x1C,
0x03,0x87,0xE0,0x00,0x00,0x18,0x1F,0xC3,0xC0,0x00,0x00,0x1B,0xFF,0xC3,0xC0,0x00,
0x00,0x3B,0x83,0x83,0x80,0x00,0x00,0x33,0x83,0x07,0xC0,0x00,0x00,0x31,0x83,0x06,
0xE0,0x20,0x00,0x61,0x87,0x8C,0x70,0x20,0x00,0x61,0xFF,0x18,0x38,0x60,0x00,0xC0,
0xE0,0x30,0x1E,0x60,0x01,0xC0,0x80,0x40,0x0F,0xE0,0x03,0x80,0x06,0x00,0x07,0xE0,
0x02,0x00,0x07,0x80,0x01,0xE0,0x04,0x00,0x03,0xC0,0x00,0x70,0x08,0x11,0x01,0xC0,
0x1C,0x00,0x00,0x31,0x80,0xC0,0x1F,0x80,0x00,0x70,0xC0,0x00,0x0F,0xC0,0x00,0x70,
0xE0,0x01,0x03,0xC0,0x00,0xE0,0x70,0x00,0x81,0xC0,0x00,0xE0,0x38,0x00,0xC0,0xC0,
0x00,0xE0,0x3E,0x00,0xE0,0x00,0x00,0xC0,0x1F,0xFF,0xF0,0x00,0x00,0x80,0x07,0xFF,
0xF8,0x00,0x00,0x00,0x01,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
"窗",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x0F,0x80,0x00,0x00,0x00,0x00,
0x07,0x80,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x00,
0x00,0x40,0x00,0x7F,0xFF,0x80,0x00,0x61,0xFF,0xE0,0x07,0xC0,0x00,0xFF,0x80,0x00,
0x07,0xE0,0x00,0xE0,0x40,0x00,0x0F,0xF0,0x01,0xC0,0x70,0x38,0x0F,0x00,0x03,0xC0,
0x78,0x1E,0x18,0x00,0x03,0x80,0xF0,0x0F,0x00,0x00,0x03,0x80,0xE0,0x07,0x80,0x00,
0x03,0x01,0x8E,0x03,0x00,0x00,0x00,0x03,0x0F,0x00,0x00,0x00,0x00,0x06,0x0F,0x80,
0x00,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x00,
0x70,0x0F,0xC0,0x00,0x00,0x10,0xC1,0xFF,0xF0,0x00,0x00,0x1F,0xFF,0x81,0xF0,0x00,
0x00,0x0E,0x06,0x00,0xF0,0x00,0x00,0x0C,0x07,0x00,0xE0,0x00,0x00,0x0C,0x0E,0x00,
0xE0,0x00,0x00,0x0C,0x0C,0x78,0xE0,0x00,0x00,0x0C,0x1F,0xFC,0xE0,0x00,0x00,0x0C,
0x30,0x3C,0xE0,0x00,0x00,0x0C,0x60,0x70,0xE0,0x00,0x00,0x0C,0x8C,0xE0,0xE0,0x00,
0x00,0x0C,0x07,0xC0,0xE0,0x00,0x00,0x0C,0x03,0xC0,0xE0,0x00,0x00,0x0C,0x07,0xE0,
0xE0,0x00,0x00,0x0C,0x1C,0xE0,0xE0,0x00,0x00,0x0C,0x38,0x20,0xE0,0x00,0x00,0x0C,
0xC0,0x00,0xE0,0x00,0x00,0x1C,0x01,0xFF,0xE0,0x00,0x00,0x1F,0xFF,0xFF,0xE0,0x00,
0x00,0x1F,0xF8,0x01,0xE0,0x00,0x00,0x0C,0x00,0x01,0xE0,0x00,0x00,0x08,0x00,0x01,
0xC0,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
"帘",
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xE0,0xE0,0xE0,
0xE0,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0xFF,0x0F,0x03,0x83,0x81,0x81,0xC0,
0xE0,0x70,0x38,0x0C,0x06,0x00,0x00,0xF8,0xF0,0x70,0x70,0x70,0x30,0x30,0x38,0x38,
0x38,0x38,0x18,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0C,0x7C,0xF8,0xF0,0xE0,0x00,0xF0,0x7F,0x00,0x00,0x03,0x07,0x07,0x03,
0x01,0x70,0xF0,0xE0,0xE0,0xE0,0xE0,0xFF,0xE3,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE0,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x20,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x00,0x00,0x00,0x7C,0xF8,0xF0,0xC0,
0x80,0x00,0x01,0x00,0xE0,0xF8,0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x84,
0xFC,0xF0,0xE0,0xE0,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x18,0x3E,0xFF,0xFC,0xF8,0xF8,0x1C,0x04,0x03,0x01,0x03,
0x03,0x00,0x00,0x00,0x00,0x03,0x07,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#endif

