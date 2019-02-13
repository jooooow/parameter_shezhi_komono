#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f4xx.h"

#define TAG PEout(8)

#define K1 PCin(4)
#define K2 PCin(5)
#define K3 PCin(0)
#define K4 PCin(1)
#define K5 PDin(10)

void InitTag(void);
void InitSwitch(void);

#endif

