#ifndef __CAN_H__
#define __CAN_H__

#include "includes.h"

void InitCan1(void);
void InitCan1NVIC(void);

void Can1SendMsg2Chassis(short ichi, short ni, short san, short yon);

#endif
