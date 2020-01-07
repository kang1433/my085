#ifndef _Include_h
#define _Include_h


#include "stm32f0xx.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include "datatype.h"
#include "Uart.h"
#include "IWDG.h"
#include "init.h"
#include "ADC.h"
#include "TASK.h"
#include "KEY.h"
#include "LCD.h"
#include "flash.h"
#include "battery.h"
#include "pwm.h"
#include "sysclock.h"
#include "vk1024b.h"


#define 	Power300W
//#define 	DEBUG_MODE		


#define	GPIO_SPEED			GPIO_Speed_50MHz
#define 	IFPrintf(A)			do{\
								if(A) printf(" *"#A"=%d\r\n",(A));\
							}\
							while(0)


/*********充电电压阀值定义*********/
#define   Diode_V				450					//单位mV
#define   N_V_Charger		(9000- Diode_V)		//单位mV
#define   C_V_Charger_Normal	(12350-Diode_V)
#define   P_V_Charger_Normal	(13200-Diode_V)		
#define   C_V_Charger_Test	(10500-Diode_V)
#define   P_V_Charger_Test	(14100-Diode_V)		

#define   N_V_SUN			(9000- Diode_V)		//单位mV
#define   C_V_SUN			(12700-Diode_V)
#define   P_V_SUN			(24500-Diode_V)	
/*********充电电流阀值定义*********/
#define   Zero_I_L			150		//ma
#define   Zero_I_H			250		//ma

#define   SUN_I_Less			3000	
#define   SUN_I_Little			4500	
#define   SUN_I_Nor 			5100	
#define   SUN_I_Pro			5700

#ifdef Power300W			
#define   CH_I_Less			3000	
#define   CH_I_Little			4500	
#define   CH_I_Nor 			5100	
#define   CH_I_Pro			5700
#else
#define   CH_I_Less			6000	
#define   CH_I_Little			9000	
#define   CH_I_Nor 			10100	
#define   CH_I_Pro			10500
#endif


/*********DC电流阀值定义*********/
#define   B_I_12V		 		30		//ma
#define   C_I_12V		 		100		
#define   P_I_12V		 		2750		

#define   B_I_DYQ		 	30		//ma
#define   C_I_DYQ		 	(200 + ReadBuf[DYQ_InitI])		
#define   P_I_DYQ		 	(7900 + ReadBuf[DYQ_InitI])		



#ifdef DEBUG_MODE
#define	my_WDG_Config()	
#define	my_SWD_In()		
#define 	mPrintf(...)			//printf(__VA_ARGS__)
#define 	mP(A) 				printf("%s=%d\n",#A,(A))
#else
#define	my_WDG_Config()	WDG_Config()
#define	my_SWD_In()		SWD_In()
#define 	mPrintf(...)	

#endif

#endif



/*生成bin文件 C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe  --bin -o   E:\PR085\DisplayBoard_STM32\USER\Objects\DisplayBoard_STM32.bin    E:\PR085\DisplayBoard_STM32\USER\Objects\DisplayBoard_STM32.axf


生成acs加密bin文件 E:\PR085\DisplayBoard_STM32\USER\Objects\Hex2Bin.exe E:\PR085\DisplayBoard_STM32\USER\Objects\DisplayBoard_STM32.hex*/

