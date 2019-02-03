#include "spi.h"
#include "delay.h"

void InitSPI4(void)
{
	u8 sta;
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_SPI4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI4, &SPI_InitStructure);
	
	SPI_Cmd(SPI4, ENABLE);
	SPI4ReadWriteByte(0xff);		
}

void InitGPIO(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&gpio);
}

u8 SPI4ReadWriteByte(u8 byte)
{
	u8 retry = 0;				 	
	u8 sta = 0;
	while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET) 	//检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry > 250)	
		{
			return 0;
		}
	}			  
	SPI_I2S_SendData(SPI4, byte); 																//通过外设SPIx发送一个数据
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry > 250) 
		{
			return 0;
		}
	}	  						    
	return SPI_I2S_ReceiveData(SPI4);
}