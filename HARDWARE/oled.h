#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f4xx.h"
#include "sys.h"
#include "spi.h"

#define FRONT_PUSH 0
#define BACK_PUSH  1
#define new_oled {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,7,8,0};

typedef struct _Oled_t
{
	void(*ClearAll)(struct _Oled_t* self);
	void(*SetPos)(struct _Oled_t* self, u8 x, u8 y);
	void(*DisplayChar)(struct _Oled_t* self, u8 x, u8 y, u8 ch);
	void(*DisplayImage)(struct _Oled_t* self, u8 x0,u8 y0,u8 x1,u8 y1,u8 img[]);
	void(*DisplayString)(struct _Oled_t* self, u8 x,u8 y,u8 *str);
	void(*ShowIcon)(struct _Oled_t* self);
	void(*PushString2BufAndDisplay)(struct _Oled_t* self, const char* str, u8 push_kata);
	
	u8 row_cnt;
	u8 max_row_cnt;
	u8 max_col_cnt;
	u8 pos_col;
	u8 buf[7][16];
}Oled_t;

void InitOled(void);

void OledClearAll(Oled_t* self);
void OledSetPos(Oled_t* self,uint8_t x,uint8_t y);
void OledDisplayChar(Oled_t* self,u8 x,u8 y,u8 ch);
void OledDisplayImage(Oled_t* self,u8 x0,u8 y0,u8 x1,u8 y1,u8 img[]);
void OledDisplayString(Oled_t* self,u8 x,u8 y,u8 *str);

void OledDisplayCharWithMask(Oled_t* self,u8 x,u8 y,u8 ch, u8 mask);
void OledDisplayStringWithMask(Oled_t* self,u8 x,u8 y,u8 *str, u8 mask);

void OledShowHLLIcon(Oled_t* self);
void OledPushString2BufAndDisplay(Oled_t* self, const char* str, u8 push_kata);

void OledWriteByte(uint8_t dat);
void OledWriteData(uint8_t dat);
void OledWriteCmd(uint8_t cmd);

#endif
