

/**********************************  STM32F0xx  *********************************
  * @�ļ���	�� LED.h
  * @����		�� 2018��09��19��
  ******************************************************************************/


#ifndef __LED_H
#define __LED_H
#include "include.h"

/* �궨�� --------------------------------------------------------------------*/

#define PORT_MU_LED1 		GPIOA
#define PIN_MU_LED1		GPIO_Pin_9

#define	MU_LED1_HIGH		GPIO_SetBits(PORT_MU_LED1,PIN_MU_LED1)//IO_SET(MU_LED1_PIN,OUTPUT_HIGH)
#define	MU_LED1_LOW		GPIO_ResetBits(PORT_MU_LED1,PIN_MU_LED1)//IO_SET(MU_LED1_PIN,OUTPUT_LOW)

/* LED�����𡢱仯 �����涨��Ч��һ��*/
#define 	LED1_ON			(PORT_LED1->BSRR = PIN_MU_LED1)
#define 	LED1_OFF			(PORT_LED1->BRR  = PIN_MU_LED1)
#define 	LED1_TOGGLE		(PORT_LED1->ODR ^= PIN_MU_LED1)


void led_init(void);

#endif


