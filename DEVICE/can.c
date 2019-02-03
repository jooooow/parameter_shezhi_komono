#include "can.h"

#define MOTOR_FRONT_LEFT  0
#define MOTOR_FRONT_RIGHT 1
#define MOTOR_BACK_LEFT   2
#define MOTOR_BACK_RIGHT  3
#define MOTOR_PITCH       4
#define MOTOR_YAW         5

void InitCan1(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	CAN_InitTypeDef        can;
	CAN_FilterInitTypeDef  can_filter;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
    
  can.CAN_TTCM = DISABLE;
  can.CAN_ABOM = DISABLE;
  can.CAN_AWUM = DISABLE;
  can.CAN_NART = DISABLE;
  can.CAN_RFLM = DISABLE;
  can.CAN_TXFP = ENABLE;
	
  can.CAN_Mode = CAN_Mode_Normal;
  can.CAN_SJW  = CAN_SJW_1tq;
  can.CAN_BS1 = 8;
  can.CAN_BS2 = 4;
  can.CAN_Prescaler = 3;   //CAN BaudRate 45/(1+9+5)/3=1Mbps
  CAN_Init(CAN1, &can);

	can_filter.CAN_FilterNumber = 0;
	can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh= 0x0000;
	can_filter.CAN_FilterIdLow = 0x0000;
	can_filter.CAN_FilterMaskIdHigh = 0x0000;
	can_filter.CAN_FilterMaskIdLow  = 0x0000;
	can_filter.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	can_filter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&can_filter);
  
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
  CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}

void InitCan1NVIC(void)
{
	NVIC_InitTypeDef       nvic;
	
	nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 2;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
}

unsigned int can1_rx_cnt_arr[6] = {0,0,0,0,0,0};
short chassis_motor_speed[6] = {0,0,0,0,0,0};
u8 wait_can_init_ok = 0;

void CAN1_RX0_IRQHandler(void)
{
	static u16 can_cnt = 0;
	CanRxMsg can1_rx_msg;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
		CAN_Receive(CAN1, CAN_FIFO0, &can1_rx_msg);	
		
		if(can_cnt < 100)
			can_cnt++;
		else
			wait_can_init_ok = 1;
		
		if(can1_rx_msg.StdId >= 0x201 && can1_rx_msg.StdId <= 0x206)
		{
			switch(can1_rx_msg.StdId)
			{
				case 0x201:
					can1_rx_cnt_arr[MOTOR_FRONT_LEFT]++;
					chassis_motor_speed[MOTOR_FRONT_LEFT] = (can1_rx_msg.Data[2]<<8)|(can1_rx_msg.Data[3]);
					break;
				case 0x202:
					can1_rx_cnt_arr[MOTOR_FRONT_RIGHT]++;
					chassis_motor_speed[MOTOR_FRONT_RIGHT] = (can1_rx_msg.Data[2]<<8)|(can1_rx_msg.Data[3]);
					break;
				case 0x203:
					can1_rx_cnt_arr[MOTOR_BACK_LEFT]++;
					chassis_motor_speed[MOTOR_BACK_LEFT] = (can1_rx_msg.Data[2]<<8)|(can1_rx_msg.Data[3]);
					break;
				case 0x204:
					can1_rx_cnt_arr[MOTOR_BACK_RIGHT]++;
					chassis_motor_speed[MOTOR_BACK_RIGHT] = (can1_rx_msg.Data[2]<<8)|(can1_rx_msg.Data[3]);
					break;
			}
		}
  }
}

void Can1SendMsg2Chassis(short ichi, short ni, short san, short yon)
{
		CanTxMsg tx_message;
    tx_message.StdId = 0x200;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    tx_message.Data[0] = (uint8_t)(ichi >> 8);
    tx_message.Data[1] = (uint8_t)ichi;
    tx_message.Data[2] = (uint8_t)(ni >> 8);
    tx_message.Data[3] = (uint8_t)ni;
    tx_message.Data[4] = (uint8_t)(san >> 8);
    tx_message.Data[5] = (uint8_t)san;
    tx_message.Data[6] = (uint8_t)(yon >> 8);
    tx_message.Data[7] = (uint8_t)yon;
    CAN_Transmit(CAN1,&tx_message);
}



