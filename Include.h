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


#define 	Power300W				//逆变功率限制300W
#define 	CHARGER_5A				//市电充电限流5A时开启定义
//#define 	CHARGE_EXCHANGE		//需对调充电口时开启定义
//#define 	DEBUG_MODE		



#define   R_ACS_S		"ACS_" 
#define   R_DCS_S		"DCS_" 
#define   R_B3S_S		"B3S_" 
#define   R_COM_S		"KCOM_" 

#define   T_ACS_S		"ACS?\r\n" 
#define   T_DCON_S		"DCON\r\n" 
#define   T_DCOFF_S		"DCOFF\r\n" 

#ifdef CHARGE_EXCHANGE
#define	SUN_PWM		0
#define	CH_PWM		1
	#ifdef Power300W
	#define   T_B3S_S		"B3S?_300W-B2\r\n" 
	#else
	#define   T_B3S_S		"B3S?_500W-B2\r\n" 
	#endif
#else
#define	SUN_PWM		1
#define	CH_PWM		0
	#ifdef Power300W
	#define   T_B3S_S		"B3S?_300W-A2\r\n" 
	#else
	#define   T_B3S_S		"B3S?_500W-A2\r\n" 
	#endif
#endif


#define	GPIO_SPEED			GPIO_Speed_50MHz
#define 	IFPrintf(A)			do{\
								if(A) printf(" *"#A"=%d\r\n",(A));\
							}\
							while(0)


/*********充电电压阀值定义*********/
#define   Diode_V				450					//单位mV
#define   N_V_Charger 		(9000- Diode_V)		//单位mV
#define   C_V_Charger_Normal (12400-Diode_V)
#define   P_V_Charger_Normal (13350-Diode_V)		
#define   C_V_Charger_Test	(10500-Diode_V)
#define   P_V_Charger_Test	(14500-Diode_V)		

#define   N_V_SUN			(9000- Diode_V)		//单位mV
#define   C_V_SUN			(12650-Diode_V)
#define   P_V_SUN			(24500-Diode_V)	
/*********充电电流阀值定义*********/
#define   Zero_I_L			150		//ma
#define   Zero_I_H			250		//ma

#define   SUN_I_Less			3000	
#define   SUN_I_Little			4500	
#define   SUN_I_Nor 			5100	
#define   SUN_I_Pro			5700

#ifdef CHARGER_5A			
#define   CH_I_Less			3000	
#define   CH_I_Little			4500	
#define   CH_I_Nor 			5100	
#define   CH_I_Pro			5700
#else
#define   CH_I_Less			6000	
#define   CH_I_Little			9000	
#define   CH_I_Nor 			10100	
#define   CH_I_Pro			10400
#endif

/*************ADC口定义*************/
#define   AD_I_12V		 	0x00	
#define   AD_I_Charge		0x01	
#define   AD_NTC		 		0x02	
#define   AD_V_Bat			0x03	

#define   AD_V_DYQ		 	0x06	
#define   AD_I_DYQ		 	0x07	

#ifdef CHARGE_EXCHANGE
#define   AD_V_Charger		0x05	//将充电口功能对调
#define   AD_V_SUN			0x04
#else
#define   AD_V_Charger		0x04	
#define   AD_V_SUN			0x05
#endif
/******************END*****************/


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

