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

#define	GPIO_SPEED			GPIO_Speed_50MHz
#define 	IFPrintf(A)			do{\
								if(A) printf(" *"#A"=%d\r\n",A);\
							}\
							while(0)

//#define 	old
//#define 	Power300W
//#define 	DEBUG_MODE		

#ifdef DEBUG_MODE
#define	my_WDG_Config()	
#define	my_SWD_In()		
#define 	mPrintf(...)			//printf(__VA_ARGS__)
#define 	mP(A) 				printf("%s=%d\n",#A,A)
#else
#define	my_WDG_Config()	WDG_Config()
#define	my_SWD_In()		SWD_In()
#define 	mPrintf(...)	

#endif

#endif



/*生成bin文件 C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe  --bin -o   E:\PR085\DisplayBoard_STM32\USER\Objects\DisplayBoard_STM32.bin    E:\PR085\DisplayBoard_STM32\USER\Objects\DisplayBoard_STM32.axf


生成acs加密bin文件 E:\PR085\DisplayBoard_STM32\USER\Objects\Hex2Bin.exe E:\PR085\DisplayBoard_STM32\USER\Objects\DisplayBoard_STM32.hex*/

