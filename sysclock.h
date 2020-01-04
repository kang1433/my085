/**************************************************************/
/* SYSCLOCK.H */
/**************************************************************/
#ifndef __SYSCLOCK_H__
#define __SYSCLOCK_H__
#include "include.h"

#define		UTMOSTTIME		65500
#define		OpDuration			3000
#define		SUNLDUTYTIME		3000
#define		CHLDUTYTIME		3000
#define		OneHour				3600000
#define		Onemin				58000
#define		CLOSETIME			(1*3600000)			//���ùػ�ʱ��Сʱ
#define		LOWPCLOSETIME		((1*60000)+29000)	//�͵����ػ�ʱ�����

#define		USB_Time		0
#define		DYQ_Time		1
#define		V12_Time		2
#define		AC_Time			3
#define		SUN_Time		4
#define		Ch_Time			5

extern __IO u8 	BuzzerTime;				//����ʱ��
extern __IO u8 	Key_PressTime;			//����ʱ��
extern __IO u16 	SW_DET_PressTime;		//�ܿ��ذ���ʱ��
extern __IO u16	UART_RX_Time;      		//���ڽ���ʱ��
extern __IO u16	B3S_RX_Time;      		
extern __IO u16	DCS_RX_Time;      		
extern __IO u16	SUN_LowDuty_Time;
extern __IO u16	CH_LowDuty_Time;
	
extern __IO u32	Open_Time; 				//����ʱ��  
extern __IO u32	LowP_Time; 			
extern __IO u32	DYQ_INIT_Time; 
extern u16	ResTime[6];
extern u16	Uptime[6];
extern u32	ACOVTime[2];


void SysTickInit(void);
//void Delay_us(u32 nTime);
//void Delay_ms(u32 i);
void SysTick_Update(void);
void SysTick_Correct(u32 timeMs);
u32 SysTimeMs(void);

void DelayMs(u16 ms);
void SYSCLKConfig_STOP(void);
#endif
