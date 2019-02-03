#ifndef __SOFTTIMER_H__
#define __SOFTTIMER_H__

#include "stm32f4xx.h"
#include "stdlib.h"

#define new_delay(a) {a,a,0,0,  NULL,  SoftTimerDescend,SoftTimerReload,  NULL};
#define Delay_t SoftTimer

typedef struct _SoftTimer
{
	unsigned short cnt;
	unsigned short reload_cnt;
	unsigned char time_out_flag;
	unsigned char old_time_out_flag;
	
	unsigned short* hook;
	
	void (*Start)(struct _SoftTimer* self);
	void (*Reload)(struct _SoftTimer* self);
	void (*OnTimeOut)(void);
	
	struct _SoftTimer* next;
}SoftTimer;

void SoftTimerDescend(SoftTimer* self);
void SoftTimerReload(SoftTimer* self);

#endif

