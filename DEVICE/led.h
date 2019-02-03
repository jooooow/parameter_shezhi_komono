#ifndef __LED_H__
#define __LED_H__

#define LED_RED_ON()  GPIO_ResetBits(GPIOE, GPIO_Pin_7)
#define LED_RED_OFF() GPIO_SetBits(GPIOE, GPIO_Pin_7)

void InitLed(void);

#endif
