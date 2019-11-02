#ifndef init_h 
#define init_h 
#include "include.h"

extern u16 Sun_Pulse;
extern u16 Charge_Dutycycle;

/* 宏定义 --------------------------------------------------------------------*/
/*AC DC使能脚*/
#define PORT_EN_USB     			GPIOB
#define PORT_EN_12V     			GPIOB
#define PORT_EN_DYQ			GPIOA
#define PORT_EN_AC_OUT   		GPIOA

#define PIN_EN_USB				GPIO_Pin_2
#define PIN_EN_12V				GPIO_Pin_5
#define PIN_EN_DYQ				GPIO_Pin_9
#define PIN_EN_AC_OUT			GPIO_Pin_15

#define USB_ENABLE				GPIO_SetBits(PORT_EN_USB,PIN_EN_USB)			
#define USB_DISABLE				GPIO_ResetBits(PORT_EN_USB,PIN_EN_USB)			
#define V12_ENABLE				GPIO_SetBits(PORT_EN_12V,PIN_EN_12V)			
#define V12_DISABLE				GPIO_ResetBits(PORT_EN_12V,PIN_EN_12V)			
#define DYQ_ENABLE				GPIO_SetBits(PORT_EN_DYQ,PIN_EN_DYQ)			
#define DYQ_DISABLE				GPIO_ResetBits(PORT_EN_DYQ,PIN_EN_DYQ)			
#define AC_OUT_ENABLE			GPIO_SetBits(PORT_EN_AC_OUT,PIN_EN_AC_OUT)			
#define AC_OUT_DISABLE			GPIO_ResetBits(PORT_EN_AC_OUT,PIN_EN_AC_OUT)			

/*电流检测恢复控制引脚*/
#define PORT_I_12V 				GPIOA

#define PIN_I_12V				GPIO_Pin_0

#define I_V12_L					GPIO_ResetBits(PORT_I_12V,PIN_I_12V)			
#define I_V12_H					GPIO_SetBits(PORT_I_12V,PIN_I_12V)			

#ifdef  old
#define PORT_FAN 				GPIOB		//旧板脚位
#define PORT_BT_STA			GPIOA		
#define PIN_FAN					GPIO_Pin_4	
#define PIN_BT_STA				GPIO_Pin_12		
#else
#define PORT_FAN 				GPIOA		
#define PORT_BT_STA			GPIOB		
#define PIN_FAN					GPIO_Pin_12		
#define PIN_BT_STA				GPIO_Pin_4		
#endif


/*设备控制脚*/
#define PORT_BZ					GPIOA
#define PORT_LIGHT 				GPIOC
#define PORT_EN_POW			GPIOB
#define PORT_LCD_LIGHT			GPIOC

#define PIN_BZ					GPIO_Pin_10
#define PIN_LIGHT				GPIO_Pin_14
#define PIN_EN_POW				GPIO_Pin_13
#define PIN_LCD_LIGHT			GPIO_Pin_15

#define FAN_ENABLE				GPIO_SetBits(PORT_FAN,PIN_FAN)			//风扇输出使能
#define FAN_DISABLE	 			GPIO_ResetBits(PORT_FAN,PIN_FAN)			
#define BUZZER_ENABLE			GPIO_SetBits(PORT_BZ,PIN_BZ)				//输出使能
#define BUZZER_DISABLE			GPIO_ResetBits(PORT_BZ,PIN_BZ)		
#define LIGHT_ENABLE			GPIO_ResetBits(PORT_LIGHT,PIN_LIGHT)		//输出使能
#define LIGHT_DISABLE			GPIO_SetBits(PORT_LIGHT,PIN_LIGHT)		//低电平有效
#define POW_ENABLE  			GPIO_SetBits(PORT_EN_POW,PIN_EN_POW)		
#define POW_DISABLE			GPIO_ResetBits(PORT_EN_POW,PIN_EN_POW)		
#define LCD_L_ENABLE			GPIO_SetBits(PORT_LCD_LIGHT,PIN_LCD_LIGHT)		
#define LCD_L_DISABLE			GPIO_ResetBits(PORT_LCD_LIGHT,PIN_LCD_LIGHT)		
#define BT_STA_ENABLE			GPIO_SetBits(PORT_BT_STA,PIN_BT_STA)		
#define BT_STA_DISABLE			GPIO_ResetBits(PORT_BT_STA,PIN_BT_STA)		

/*检测脚*/
#define PORT_CH_SHORT 			GPIOA
#define PORT_USB_ERROR      		GPIOB
#define PORT_USB_ERROR2    		GPIOA
#define PORT_USB_INPUT			GPIOB
#define PORT_12V1_INPUT		GPIOB
#define PORT_12V2_INPUT		GPIOB
#define PORT_DYQ_PG	 		GPIOB

#define PIN_CH_SHORT			GPIO_Pin_7
#define PIN_USB_ERROR			GPIO_Pin_15
#define PIN_USB_ERROR2			GPIO_Pin_1
#define PIN_USB_INPUT			GPIO_Pin_3
#define PIN_12V1_INPUT			GPIO_Pin_9
#define PIN_12V2_INPUT			GPIO_Pin_8
#define PIN_DYQ_PG				GPIO_Pin_14

#define CH_SHORT_L				GPIO_ResetBits(PORT_CH_SHORT,PIN_CH_SHORT)			
#define GET_CH_SHORT  			GPIO_ReadInputDataBit(PORT_CH_SHORT,PIN_CH_SHORT)	//充电短路检测
#define GET_USB_ERROR			GPIO_ReadInputDataBit(PORT_USB_ERROR,PIN_USB_ERROR)
#define GET_USB_ERROR2			GPIO_ReadInputDataBit(PORT_USB_ERROR2,PIN_USB_ERROR2)
#define GET_USB_INPUT			GPIO_ReadInputDataBit(PORT_USB_INPUT,PIN_USB_INPUT)	
#define GET_12V1_INPUT			GPIO_ReadInputDataBit(PORT_12V1_INPUT,PIN_12V1_INPUT)	
#define GET_12V2_INPUT			GPIO_ReadInputDataBit(PORT_12V2_INPUT,PIN_12V2_INPUT)	
#define GET_DYQ_PG				GPIO_ReadInputDataBit(PORT_DYQ_PG,PIN_DYQ_PG)	



void PORTInit(void);
void SWD_In(void);

void CH_SHORT_OUT_L(void);
void CH_SHORT_IN(void);
void I_12V_PullL(void);
void I_12V_SetAn(void);
void AC_OUT_Op(u8 sta );
void V12_Op(u8 sta );
void DYQ_Op(u8 sta );
void UBS_Op(u8 sta )		;
void POW_Op(u8 sta);
void POW_CH_Op(u8 sta);
void K_memset(u8 num,u8* p_counter,u8 size);
u16 ChartoDecimal(u8 *src);
#endif
