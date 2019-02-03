#include "includes.h"
#include "spi.h"
#include "oled.h"
#include "fifo.h"
#include "string.h"
#include "stdio.h"
#include "menu.h"

#define MODE_SET   0
#define MODE_DEBUG 1

Oled_t oled  = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,7,16,0};
Oled_t oled_arrow  = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,6,2,0};
Oled_t oled_left  = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,6,7,8};
Oled_t oled_right = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,6,7,72};

extern MenuNode_t control;
extern MenuNode_t dp_para;
extern MenuNode_t yt_para;
extern MenuNode_t bl_para;
extern MenuNode_t gs_para;
extern MenuNode_t ny_para;
extern MenuNode_t sample1;

u8 mode = MODE_SET;

int main()
{
	u8 i,j;
	u8 temp[4];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	InitUsart3();
	InitUsart3NVIC();
	printf("init..\r\n");
	
	InitSPI4();
	InitGPIO();
	InitOled();
	
	delay_ms(100);
	
	InitMenu();
	InitTimer3();
  InitTimer3NVIC();
	
	oled.ShowIcon(&oled);
	for(i = 0; i < 2; i++)
	{
		oled.DisplayString(&oled,44,6,"init  ");
		delay_ms(300);
		oled.DisplayString(&oled,44,6,"init. ");
		delay_ms(300);
		oled.DisplayString(&oled,44,6,"init..");
		delay_ms(300);
	}
	printf("InitOled done \r\n");
	
	oled.DisplayString(&oled,44,6,"      ");
	oled.DisplayString(&oled,5,6,"K1:set K4:debug");
	while(1)
	{
		if(K1 == 0)
		{
			mode = MODE_SET;
			break;
		}
		else if(K4 == 0)
		{
			mode = MODE_DEBUG;
			break;
		}
	}
	
	oled.ClearAll(&oled);
	
	if(mode == MODE_SET)
	{
		printf("into mode set\r\n");
		oled.DisplayString(&oled,0,6,"                ");
		oled.DisplayString(&oled,0,3,"[HLL Infantry]");
		delay_ms(1600);
		oled.DisplayString(&oled,0,3,"                ");
		
		oled_arrow.DisplayChar(&oled_arrow,0,0,'>');
		oled_left.PushString2BufAndDisplay(&oled_left,control.name,BACK_PUSH);
		oled_left.PushString2BufAndDisplay(&oled_left,dp_para.name,BACK_PUSH);
		oled_left.PushString2BufAndDisplay(&oled_left,yt_para.name,BACK_PUSH);
		oled_left.PushString2BufAndDisplay(&oled_left,bl_para.name,BACK_PUSH);
		oled_left.PushString2BufAndDisplay(&oled_left,gs_para.name,BACK_PUSH);
		oled_left.PushString2BufAndDisplay(&oled_left,ny_para.name,BACK_PUSH);
		DisplayChildren(&control);
		DisplayDiv();
		TIM_Cmd(TIM3,ENABLE);
	}
	else if(mode == MODE_DEBUG)
	{
		printf("into mode debug\r\n");
		oled.DisplayString(&oled,0,6,"                ");
		oled.DisplayString(&oled,0,3,"[Debug]");
		delay_ms(1600);
		oled.DisplayString(&oled,0,3,"                ");
	}
	while(1)
	{
		
	}
	return 0;
}
