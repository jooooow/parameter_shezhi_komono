extern "C"
{
#include "control_task.h"
}
#include "stdio.h"
#include "spi.h"
#include "softtimer.h"
#include "menu2.h"

void InitControlTask(void)
{
	//InitSwitch();
	//InitUsart3();
}

u8 k_sta[5] = {1,1,1,1,1};
u8 old_k_sta[5] = {1,1,1,1,1};

extern Menu* menu_ptr;

void ControlTask(void)
{
	u8 i;
	u8 pushed_key = Menu::UNKNOWN;

	k_sta[0] = K1;
	k_sta[1] = K2;
	k_sta[2] = K3;
	k_sta[3] = K4;
	k_sta[4] = K5;
	
	if(!k_sta[0] && old_k_sta[0])
		pushed_key = Menu::LEFT;
	else if(!k_sta[1] && old_k_sta[1])
		pushed_key = Menu::UP;
	else if(!k_sta[2] && old_k_sta[2])
		pushed_key = Menu::DOWN;
	else if(!k_sta[3] && old_k_sta[3])
		pushed_key = Menu::RIGHT;
	else if(!k_sta[4] && old_k_sta[4])
		pushed_key = Menu::KAKUNIN;
	
	menu_ptr->Move(pushed_key);
	
	old_k_sta[0] = k_sta[0];
	old_k_sta[1] = k_sta[1];
	old_k_sta[2] = k_sta[2];
	old_k_sta[3] = k_sta[3];
	old_k_sta[4] = k_sta[4];
}