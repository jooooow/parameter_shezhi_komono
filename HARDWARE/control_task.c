#include "control_task.h"
#include "spi.h"
#include "softtimer.h"
#include "menu.h"
#include "oled.h"

//控制周期 单位ms
#define GIMBAL_CONTROL_CYCLE  1
#define CHASSIS_CONTROL_CYCLE 4
#define _max(a,b) (((a) > (b)) ? (a) : (b))
#define _min(a,b) (((a) < (b)) ? (a) : (b))

extern MenuNode_t control;
Menu_t menu = {&control,&control};

void InitControlTask(void)
{
	InitSwitch();
	InitUsart3();
}

extern u8 wait_can_init_ok;

u8 i = 0;

Delay_t oled_timer = new_delay(50);
Delay_t switch_timer = new_delay(2);
Delay_t solid_timer = new_delay(100);

u8 k_sta[5] = {1,1,1,1,1};
u8 old_k_sta[5] = {1,1,1,1,1};

extern Oled_t oled;
extern u8 usart3_set_recv_flag;
extern u8 usart3_set_temp_buf[16];
extern u8 wait_host_kakunin_flag;
extern MenuNode_t control_reset;

void ControlTask(void)
{
	u8 i;

	k_sta[0] = K1;
	k_sta[1] = K2;
	k_sta[2] = K3;
	k_sta[3] = K4;
	k_sta[4] = K5;
	
	if(control_reset.value.us == 1)
	{
		printf("reset self\r\n");
		__set_FAULTMASK(1);
		NVIC_SystemReset();
	}
	
	if(usart3_set_recv_flag)
	{
		usart3_set_recv_flag = 0;
		
		if(strcmp(usart3_set_temp_buf, "OK") == 0)
		{
			printf("recv host kakunin!\r\n");
			if(wait_host_kakunin_flag)
			{
				printf("kakunin ok\r\n");
				if(wait_host_kakunin_flag > 0)
					wait_host_kakunin_flag--;
				if(wait_host_kakunin_flag == 0)
				{
					oled.DisplayString(&oled,0,6,"                ");
				}
			}
		}
		else if(strcmp(usart3_set_temp_buf, "RESET") == 0)
		{
			printf("reset self\r\n");
			__set_FAULTMASK(1);
			NVIC_SystemReset();
		}
		else
		{
			printf("unidentified cmd!\r\n");
		}
	}
	
	Move(&menu);
	
	old_k_sta[0] = k_sta[0];
	old_k_sta[1] = k_sta[1];
	old_k_sta[2] = k_sta[2];
	old_k_sta[3] = k_sta[3];
	old_k_sta[4] = k_sta[4];
}
