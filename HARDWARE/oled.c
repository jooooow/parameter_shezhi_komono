#include "oled.h"
#include "string.h"
#include "delay.h"

u8 oled_temp[16];

extern unsigned char F6x8[][6];

extern u8 gImage_hll_icon_data[378];

void OledWriteByte(uint8_t dat)
{
    SPI4ReadWriteByte(dat);
}

void OledWriteData(uint8_t dat)
{
    CS = 0;
    DC = 1;
    OledWriteByte(dat);
}

void OledWriteCmd(uint8_t cmd)
{
    CS = 0;
		DC = 0;
    OledWriteByte(cmd);
}

void InitOled(void)
{
    REST = 1;
    delay_ms(200);
    REST = 0;
    delay_ms(200);
    REST = 1; 

    OledWriteCmd(0xAE);
    OledWriteCmd(0x00);
    OledWriteCmd(0x10);
    OledWriteCmd(0x40);
    OledWriteCmd(0x81);
    OledWriteCmd(0xCF);
    OledWriteCmd(0xA1);
    OledWriteCmd(0xC8);
    OledWriteCmd(0xA6);
    OledWriteCmd(0xA8);
    OledWriteCmd(0x3f);
    OledWriteCmd(0xD3);
    OledWriteCmd(0x00);
    OledWriteCmd(0xd5);
    OledWriteCmd(0x80);
    OledWriteCmd(0xD9);
    OledWriteCmd(0xF1);
    OledWriteCmd(0xDA);
    OledWriteCmd(0x12);
    OledWriteCmd(0xDB);
    OledWriteCmd(0x40);
    OledWriteCmd(0x20);
    OledWriteCmd(0x02);
    OledWriteCmd(0x8D);
    OledWriteCmd(0x14);
    OledWriteCmd(0xA4);
    OledWriteCmd(0xA6);
    OledWriteCmd(0xAF);

    OledWriteCmd(0xAF); /*display ON*/
		OledClearAll(NULL);
		OledSetPos(NULL,0,0);
}  

void OledSetPos(Oled_t* self,uint8_t x,uint8_t y)
{
    OledWriteCmd(0xb0+y);
    OledWriteCmd(((x&0xf0)>>4)|0x10);
    OledWriteCmd((x&0x0f)|0X01);
}

void OledClearAll(Oled_t* self)
{
    u8 page,len;
    for(page=0;page<8;page++)
    {
        OledWriteCmd(0xB0+page);
        OledWriteCmd(0X02);
        OledWriteCmd(0X10);

        for(len=0;len<128;len++)
            OledWriteData(0);
    }
}

void OledDisplayChar(Oled_t* self,u8 x,u8 y,u8 ch)
{
    u8 i=0;
		u8 ret=0;
    ret = ch - ' ';
    if(x > 128-1)
    {
			x = 0;
			y = y + 2;
    }
   
    OledSetPos(self,x,y+1);
    for(i=0;i<6;i++)
    {
			//OledWriteData(F6x8[ret][i]);
		}
}

void OledDisplayImage(Oled_t* self,u8 x0,u8 y0,u8 x1,u8 y1,u8 img[])
{
    u8 x,y;
    u32 i=0;

    if(y1%8==0)
        y = y1 / 8;
    else
        y = y1 /8 + 1;
    for(y=y0;y<y1;y++)
    {
			OledSetPos(self,x0,y);
			for(x=x0;x<x1;x++)
			{
				//OledWriteData(img[i++]);
			}
    }
}

void OledDisplayString(Oled_t* self,u8 x,u8 y,u8 *str)
{
    u8 i=0;

    while(str[i]!='\0')
    {
			self->DisplayChar(self,x,y,str[i]);
			x += 8;
			if(x>120)
			{
				x = 0;
				y += 2;
			}
			i++;
		}
}

void OledShowHLLIcon(Oled_t* self)
{
	OledDisplayImage(self,37,0,91,7,gImage_hll_icon_data);
}

void OledPushString2BufAndDisplay(Oled_t* self, const char* str, u8 push_kata)
{
	u8 i,j;
	//printf("%s %d\r\n",str,strlen(str));
	u8 str_len = strlen(str) > self->max_col_cnt ? self->max_col_cnt : strlen(str);
	
	
		
	if(self->row_cnt < self->max_row_cnt)
	{
		if(push_kata == BACK_PUSH)
		{
			memcpy(self->buf + self->row_cnt,str,str_len);
			memset(*(self->buf + self->row_cnt) + str_len,' ',self -> max_col_cnt - str_len);
			self->row_cnt++;
		}
		else if(push_kata == FRONT_PUSH)
		{
			for(i = self->max_row_cnt - 1; i >= 1; i--)
			{
				memcpy(self->buf + i,self->buf+i - 1,self->max_col_cnt);
			}
			memcpy(self->buf + 0,str,str_len);
			memset(*(self->buf + 0) + str_len,' ',self -> max_col_cnt - str_len);
			self->row_cnt++;
		}
	}
	else
	{
		if(push_kata == BACK_PUSH)
		{
			for(i = 0; i < self->max_row_cnt - 1; i++)
			{
				memcpy(self->buf+i,self->buf+i + 1,self->max_col_cnt);
			}
			memcpy(self->buf + self->max_row_cnt - 1,str,str_len);
			memset(*(self->buf + self->max_row_cnt - 1) + str_len,' ',self->max_col_cnt - str_len);
		}
		else if(push_kata == FRONT_PUSH)
		{
			for(i = self->max_row_cnt - 1; i >= 1; i--)
			{
				memcpy(self->buf + i,self->buf+i - 1,self->max_col_cnt);
			}
			memcpy(self->buf + 0,str,str_len);
			memset(*(self->buf + 0) + str_len,' ',self->max_col_cnt - str_len);
		}
	}
	
	for(i = 0; i < self->row_cnt; i++)
	{
		memcpy(oled_temp,self->buf+i,self->max_col_cnt);
		self->DisplayString(self,self->pos_col,i,oled_temp);
	}
}

void OledDisplayCharWithMask(Oled_t* self,u8 x,u8 y,u8 ch, u8 mask)
{
	 u8 i=0;
	 u8 ret=0;
   ret = ch - ' ';
   if(x > 128-1)
   {
	 	x = 0;
	 	y = y + 2;
   }
   
   OledSetPos(self,x,y+1);
   for(i=0;i<6;i++)
   {
//		if(mask == 1)
//			OledWriteData(~(F6x8[ret][i]));
//		else
//			OledWriteData(F6x8[ret][i]);
	 }
}

void OledDisplayStringWithMask(Oled_t* self,u8 x,u8 y,u8 *str, u8 mask)
{
	u8 i=0;
	u8 cnt = 0;

	while(str[i]!='\0' && cnt < 7)
	{
		if(i == mask)
			OledDisplayCharWithMask(self,x,y,str[i],1);
		else
			OledDisplayCharWithMask(self,x,y,str[i],0);
		x += 8;
		if(x>120)
		{
			x = 0;
			y += 2;
		}
		i++;
		cnt++;
	}
}
