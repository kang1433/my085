#ifndef _key_h
#define _key_h

#include "include.h"

#define PORT_KEY		GPIOF
#define PORT_SW_DET 	GPIOF

#define PIN_KEY			GPIO_Pin_1
#define PIN_SW_DET		GPIO_Pin_0

#define GET_KEY			GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY)
#define GET_SW_DET		GPIO_ReadInputDataBit(PORT_SW_DET,PIN_SW_DET)

void KyeConfig(void);
void CheckSWDET(void);
#endif
