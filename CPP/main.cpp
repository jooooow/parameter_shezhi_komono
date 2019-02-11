#include "oled2.h"
#include "VerticalLayout.h"
#include "menu2.h"

extern "C"
{
#include "stdlib.h"
#include "stdio.h"
#include "delay.h"
#include "usart3.h"
#include "stdio.h"
#include "gpio.h"
}

#define MODE_SET   0
#define MODE_DEBUG 1

static void Init();
static u8 WaitToGetMode();
static u8 SetLayout(u8 mode);
static void KajyouAnime(u8 mode);

u8 komono_mode = MODE_SET;
Oled* oled_ptr;
Menu* menu_ptr;

extern MenuNode_t control;
extern MenuNode_t dp_para;
extern MenuNode_t yt_para;
extern MenuNode_t bl_para;
extern MenuNode_t gs_para;

int main()
{
	menu_ptr = new Menu();
	
	/*初始化*/
	Init();
	
	/*初始化oled*/
	oled_ptr = new Oled(128,64);
	
	/*等待选择模式*/
	komono_mode = WaitToGetMode();
	
	/*设置布局器*/
	SetLayout(komono_mode);
	
	/*过场动画*/
	KajyouAnime(komono_mode);
	
	u8 cnt = 0;
	
	for(int i = 0; i < 6; i++)
		oled_ptr->ShowChar(8*8,i,'|');
	
	InitTimer3();
	InitTimer3NVIC();
	TIM_Cmd(TIM3,ENABLE);
	
	oled_ptr->Push2Left(control.name,1);
	oled_ptr->Push2Left(dp_para.name,1);
	oled_ptr->Push2Left(yt_para.name,1);
	oled_ptr->Push2Left(bl_para.name,1);
	oled_ptr->Push2Left(gs_para.name,1);
	oled_ptr->ShowChar(0,0,'>');
	menu_ptr->ShowChildren(oled_ptr);
	
	oled_ptr->Update();
	
	while(1)
	{

	}
	return 0;
}

static void Init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	InitUsart3();
	InitUsart3NVIC();
	printf("init..\r\n");
}

static u8 WaitToGetMode()
{
	while(1)
	{
		if(K1 == 0)
		{
			return MODE_SET;
		}
		else if(K4 == 0)
		{
			return MODE_DEBUG;
		}
	}
}

static u8 SetLayout(u8 mode)
{
	if(mode == MODE_SET)
	{
		VerticalLayout* layout_left = new VerticalLayout(7,5,1);
		VerticalLayout* layout_right = new VerticalLayout(7,5,1);
		oled_ptr->SetLayout(layout_left,layout_right);
	}
	else if(mode == MODE_DEBUG)
	{
		VerticalLayout* layout_left = new VerticalLayout(7,6,1);
		oled_ptr->SetLayout(layout_left,NULL);
	}
}

static void KajyouAnime(u8 mode)
{
	if(mode == MODE_SET)
	{
		oled_ptr->ClearAll();
		printf("-into mode set\r\n");
		oled_ptr->ShowString(0,6,"                 ");
		oled_ptr->ShowString(0,3,"[HLL Infantry]");
		delay_ms(1600);
		oled_ptr->ShowString(0,3,"                ");
	}
	else if(mode == MODE_DEBUG)
	{
		oled_ptr->ClearAll();
		printf("-into mode debug\r\n");
		oled_ptr->ShowString(0,6,"                 ");
		oled_ptr->ShowString(0,3,"[DEBUG]");
		delay_ms(1600);
		oled_ptr->ShowString(0,3,"       ");
	}
}