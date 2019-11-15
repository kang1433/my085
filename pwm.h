
#ifndef __PWM_H
#define __PWM_H
#include "include.h"

//#define ARR 			125
#define FULL_Dutycycle 	100
#define START_Dutycycle	5
#define Middle_Dutycycle	80
#define DOWN_Dutycycle 	0
#define PR_Dutycycle 		30


#define TIM1_COUNTER_CLOCK		12000000                 //计数时钟(12M次/秒)
#define TIM1_PRESCALER_VALUE		(SystemCoreClock/TIM1_COUNTER_CLOCK - 1)
#define TIM1_FREQUENCY				250
#define TIM1_PERIOD					(uint16_t)(TIM1_COUNTER_CLOCK/TIM1_FREQUENCY - 1)

#define PINSOURCE_Charge		GPIO_PinSource11
#define PINSOURCE_SUN			GPIO_PinSource8

#define PORT_EN_Charge			GPIOA
#define PORT_EN_SUN			GPIOA

#define PIN_EN_Charge			GPIO_Pin_11
#define PIN_EN_SUN				GPIO_Pin_8

void Charge_IOInit(void);
void TIM1_CH1_PWM_Init(void);
void Charger_Op(u8 sta, u8 Channel,u8 Dutycycle);

#endif


