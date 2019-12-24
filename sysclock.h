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
#define		CLOSETIME			(1*3600000)			//放置关机时间小时
#define		LOWPCLOSETIME		((1*60000)+29000)	//低电量关机时间分钟

#define		USB_Time		0
#define		DYQ_Time		1
#define		V12_Time		2
#define		AC_Time			3
#define		SUN_Time		4
#define		Ch_Time			5

extern u8 	BuzzerTime;				//鸣叫时间
extern u8 	Key_PressTime;			//按键时间
extern u16 	SW_DET_PressTime;		//总开关按下时间
extern u16	UART_RX_Time;      		//串口接收时间
extern u16	B3S_RX_Time;      		
extern u16	DCS_RX_Time;      		
extern u16	SUN_LowDuty_Time;
extern u16	CH_LowDuty_Time;
	
extern u32	Open_Time; 				//开机时间  
extern u32	LowP_Time; 			
extern u32	DYQ_INIT_Time; 			
extern u16	ResTime[];
extern u16	Uptime[];
extern u32	ACOVTime[];



void SysTickInit(void);
//void Delay_us(u32 nTime);
//void Delay_ms(u32 i);
void SysTick_Update(void);
void SysTick_Correct(u32 timeMs);
u32 SysTimeMs(void);

void DelayMs(u16 ms);
void SYSCLKConfig_STOP(void);
#endif
