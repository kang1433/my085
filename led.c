

#include "led.h"


void led_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_MU_LED1; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(PORT_MU_LED1,&GPIO_InitStructure); 			
	MU_LED1_LOW;
}



