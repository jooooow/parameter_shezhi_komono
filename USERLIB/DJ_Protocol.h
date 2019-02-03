#ifndef _DJ_PROTOCOL_H_
#define _DJ_PROTOCOL_H_

#include "stm32f4xx.h"


/***********************************************
ʹ�÷�����
1����ʼ�����ڣ� ʵ�ִ��ڷ��ͺ���
2�����ĺ궨�� USART_SendBuff �������Ϊʵ�ʴ��ڷ��ͺ�����  xxxxxx   ���ú�������Ϊ uchar *buff,uchar len    lenΪ���ͳ��ȣ�
3������ tCmdID  cmdid = SelfDefinedData; //�Զ������� 
				tSelfDefineInfo SelfDefineInfo = {.data1 = 1.133f,.data2 = 1.133f,.data3 = 1.143f};//�Զ��崫������
   ���ú��� Send_FrameData(cmdid,(uint8_t *)&SelfDefineInfo,sizeof(SelfDefineInfo));  //�����Զ�������������ϵͳ
4�������Ҫ���ղ���ϵͳ���������ݣ���Ҫʵ�ִ����жϽ��պ��������ձ�־Ϊ USART2_RX_STA�������ַ USART2_RX_BUF
	Ȼ������������� DataVerify() ������Ȼ���������һ�����ڷ��͵��������ݹ۲죨����ʵ��printf��������
	--------------------------------------------------------------------------2017/6/18
***********************************************/


// uart ͨ�����ã������� 115200, ����λ 8, ֹͣλ 1������λ�ޣ��������ޡ�
// --------------------------------------------------------------------------------
//| FrameHeader(5-Byte) | CmdID(2-Byte) | Data(n-Byte) | FrameTail(2-Byte, CRC16) |
// --------------------------------------------------------------------------------
//crc8 generator polynomial:G(x)=x8+x5+x4+1

#define USART_SendBuff   USART_Send_BUFF  //USART_Send_BUFF(uchar *buff,uchar len) 

/********************  ��������  ***************************/
//0-3 bits
#define Strike_0   0x00 //0��װ���棨ǰ��
#define Strike_1   0x01 //1��װ���棨��
#define Strike_2   0x02 //2��װ���棨��
#define Strike_3   0x03 //3��װ���棨�ң�
#define Strike_4   0x04 //4��װ���棨��1��
#define Strike_5   0x05 //5��װ���棨��2��
//4-7 bits
#define BC_Strike          0x00 //װ���˺����ܵ������� BloodChanged    BC
#define BC_ShootSpeed      0x01 //�ӵ����ٿ�Ѫ
#define BC_ShootFreq       0x02 //�ӵ���Ƶ��Ѫ
#define BC_PowerLimit      0x03 //���ʳ���
#define BC_ModuleOffline   0x04 //ģ�����߿�Ѫ
#define BC_CommonFoul      0x06 //��ͨ�����Ѫ
#define BC_Tarmac          0x0a //ͣ��ƺ��Ѫ
#define BC_EngineerOuto    0x0b //���̻������Զ���Ѫ


typedef __packed struct
{
	uint8_t SOF;          //������ʼ�ֽڣ��̶�Ϊ0xA5          
	uint16_t DataLength;  //���ݳ���
	uint8_t Seq;          //�����
	uint8_t CRC8;         //֡ͷCRCУ��
}tFrameHeader;//֡ͷ

typedef enum 
{
	GameRobotState = 0x0001,     //����������Ϣ    ����Ƶ�� 50 Hz
	RobotHurt=0x0002,  //ʵʱѪ���仯����
	ShootData=0x0003,             //ʵʱ�������
	PowerHeartData=0x0004,        //ʵʱ�������� (0��0004)
	RfidDetect=0x0005,   //���ؽ�������(0x0005)
	GameResult=0x0006,   //����ʤ������(0x0006)
	GetBuff=0x0007,			//Buff��ȡ����(0X0007)
	GameRobotPos=0x0008,   //������λ�ó�����Ϣ(0X0008)
	ShowData=0x0100,   //�������Զ�������(0x0100)
	Wrong = 0x1301 //ö����Ч��ֻ��Ϊ��ʹ��ö�ٴ�СΪ2�ֽ�
}tCmdID;

//typedef __packed struct
//{
//	uint8_t flag;
//	uint32_t x;
//	uint32_t y;
//	uint32_t z;
//	uint32_t compass; //��λ
//}tLocData; //λ����Ϣ

typedef __packed struct
{
	uint16_t stageRemainTime;       //����ʣ��ʱ�䣨�ӵ���ʱ�����ӿ�ʼ���㣬��λ s��
	uint8_t gameProgress; //��ǰ�����׶�
	uint8_t robotLevel;    //�����˵�ǰ�ȼ�
	uint16_t remainHP;    //�����˵�ǰѪ��
	uint16_t maxHP;         //��������Ѫ��
}extGameRobotState_t; //����������״̬��0x0001��

typedef __packed struct
{
	uint8_t armorType:4;
	uint8_t hurtType:4;
}extRobotHurt_t;//�˺����ݣ�0x0002��

typedef __packed struct
{
	uint8_t bulletType; //��������
	uint8_t bulletFreq;  //�ӵ�ʵʱ��Ƶ���� /s��
	float bulletSpeed;   //��������(m/s)
}extShootData_t;    //ʵʱ�����Ϣ (0��0003)

typedef __packed struct
{
	float chassisVolt; //���������ѹ
	float chassisCurrent; //�����������
	float chassisPower; //�����������
	float chassisPowerBuffer; //���̹��ʻ���
	uint16_t shooterHeat0; //17mmǹ������
	uint16_t shooterHeat1; //42mmǹ������
}extPowerHeartData_t;    //ʵʱ�������� (0��0004)

typedef __packed struct
{
	uint8_t cardType;  //������
	uint8_t cardldx;   //��������
}extRfidDetect_t;   //���ؽ�������(0x0005)

typedef __packed struct
{
	uint8_t winner;

}extGameResult_t;   //����ʤ������(0x0006)

typedef __packed struct
{
	uint8_t buffType;  //Buff����
	uint8_t buffAddition;   //�ӳɰٷֱ�
}extGetBuff_t;   //Buff��ȡ����(0X0007)

typedef __packed struct
{
	float x;  //λ��x����ֵ
	float y;  //λ��y����ֵ
	float z;  //λ��z����ֵ
	float yaw;  //ǹ�ڳ���Ƕ�ֵ
}extGameRobotPos_t;   //������λ�ó�����Ϣ(0X0008)

typedef __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
}extShowData_t;   //�������Զ�������(0x0100).

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	__packed union 
	{
		extGameRobotState_t       GameRobotState;    //����������Ϣ ��0x0001��
		extRobotHurt_t   RobotHurt; //ʵʱѪ���仯���ݣ�0x0002��
		extShootData_t   ShootData; //ʵʱ�����Ϣ (0��0003)
		extPowerHeartData_t   PowerHeartData ;    //ʵʱ�������� (0��0004)
		extRfidDetect_t    RfidDetect;   //���ؽ�������(0x0005)
		extGameResult_t    GameResult;   //����ʤ������(0x0006)
		extGetBuff_t   GetBuff;   //Buff��ȡ����(0X0007)
		extGameRobotPos_t    GameRobotPos;   //������λ�ó�����Ϣ(0X0008)
		extShowData_t     ShowData;   //�������Զ�������(0x0100).
	}Data;
	uint16_t        CRC16;         //֮ǰ��������CRCУ��   ע������ݺ�֮ǰ�����ݿ��ܲ����������Բ�Ҫֱ��ʹ�ã�����Ҫֱ��ʹ�ã������ڴ˸�ֵ
}tFrame;  //����֡

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extGameRobotState_t     GameRobotState;   
	uint16_t        CRC16;         //����CRCУ��
}extGameRobotState_tFrame;  //����������Ϣ֡ ��0x0001��

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extRobotHurt_t   RobotHurt;   
	uint16_t        CRC16;         //����CRCУ��
}extRobotHurt_tFrame; //ʵʱѪ���仯���ݣ�0x0002��

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extShootData_t   ShootData;  
	uint16_t        CRC16;         //����CRCУ��
}extShootData_tFrame;    //ʵʱ�����Ϣ (0��0003)

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extPowerHeartData_t   PowerHeartData ;   
	uint16_t        CRC16;         //����CRCУ��
}extPowerHeartData_tFrame;  ////ʵʱ�������� (0��0004)

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extRfidDetect_t    RfidDetect;   
	uint16_t        CRC16;         //����CRCУ��
}extRfidDetect_tFrame;    //���ؽ�������(0x0005)

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extGameResult_t   GameResult;   
	uint16_t        CRC16;         //����CRCУ��
}extGameResult_tFrame;    //����ʤ������(0x0006)

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extGetBuff_t   GetBuff;   
	uint16_t        CRC16;         //����CRCУ��
}extGetBuff_tFrame;    //Buff��ȡ����(0X0007)

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extGameRobotPos_t   GameRobotPos;   
	uint16_t        CRC16;         //����CRCУ��
}extGameRobotPos_tFrame;    //������λ�ó�����Ϣ(0X0008)

typedef __packed struct
{
	tFrameHeader    FrameHeader;
	tCmdID          CmdID;
	extShowData_t     ShowData;   
	uint16_t        CRC16;         //����CRCУ��
}extShowData_tFrame;    //�������Զ�������(0x0100).

u8 DataVerify(void);
// ʹ�÷��� �����û����� Send_FrameData(SelfDefinedDara, userMessage,tSelfDefineInfo(userMessage)); 
void Send_FrameData(tCmdID cmdid, uint8_t * pchMessage,uint8_t dwLength); 


#endif

