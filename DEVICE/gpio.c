#include "gpio.h"

void InitTag(void)
{
	GPIO_InitTypeDef gpio;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_8;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&gpio);
}

void InitSwitch(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&gpio);
}
