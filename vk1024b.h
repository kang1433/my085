/**
  ******************************************************************************
  * @file    Vk1024B.h 
  * @author  kevin_guo
  * @version V1.0.0
  * @date    05-17-2018
  * @brief   Vk1024B program body
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VK1024B_H
#define __VK1024B_H

/* Includes ------------------------------------------------------------------*/
//此包含文件根据客户单片机做相应的修改
#include "include.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
//基本命令
#define OSC_OFF    	0x00         				// 关闭震荡器
#define OSC_ON    	0x01         				// 开启震荡器
#define DISP_OFF 	0x02					// 关LCD Bias
#define DISP_ON   	0x03         				// 开LCD Bias
#define COM_1_3__4  0x29  					// 1/3bias 4com
#define COM_1_3__3  0x25  					// 1/3bias 3com
#define COM_1_3__2  0x21  					// 1/3bias 2com
#define COM_1_2__4  0x28  					// 1/2bias 4com
#define COM_1_2__3  0x24  					// 1/2bias 3com
#define COM_1_2__2  0x20  					// 1/2bias 2com
//扩展命令，该功能未引出关闭可降低功耗
#define TIMER_DIS	0x04         				// 
#define WDT_DIS   	0x05         				// 
#define BUZZ_OFF  	0x08         				// 关闭蜂鸣器 
#define RC32K  		0X18         			
#define IRQ_DIS  	0X80        			 
#define Prejudge		1

/*定义LCD中各字段显示状态在Display_Sta中BIT 位置*/
#define		V_C_LEFT					0
#define		V_BAT_20					1
#define		V_FRAME_OUT 				2
#define		V_D_DOWN					3 	
#define		V_USB						4 
#define		V_AC_CH 					5 
#define		V_C_DOWN					6
#define		V_BAT_40					7
#define		V_BAT_100					8
#define		V_D_LEFT					9
#define		V_DC12						10
#define		V_SUN_CH					11
#define		V_C_UP						12
#define		V_BAT_60					13
#define		V_FRAME_BAT 				14
#define		V_D_RIGHT					15
#define		V_SMOKE 					16
#define		V_FRAME_IN					17
#define		V_C_RIGHT					18
#define		V_BAT_80					19
#define		V_WARNING					20
#define		V_D_UP						21
#define		V_AC_Out					22


//驱动seg数
#define 	VK1024B_SEGNUM		6

//以下管脚配置根据客户单片机做相应的修改 
#define VK1024B_CS_PIN				GPIO_Pin_12
#define VK1024B_CS_GPIO_PORT		GPIOB
#define VK1024B_CS_GPIO_CLK		RCC_APB2Periph_GPIOB

#define VK1024B_WR_PIN				GPIO_Pin_11
#define VK1024B_WR_GPIO_PORT        GPIOB
#define VK1024B_WR_GPIO_CLK           RCC_APB2Periph_GPIOB

#define VK1024B_DAT_PIN			GPIO_Pin_10
#define VK1024B_DAT_GPIO_PORT       GPIOB
#define VK1024B_DAT_GPIO_CLK          RCC_APB2Periph_GPIOB

//以下管脚输出定义根据客户单片机做相应的修改
#define VK1024B_WR_H() VK1024B_WR_GPIO_PORT ->ODR |=  VK1024B_WR_PIN
#define VK1024B_WR_L() VK1024B_WR_GPIO_PORT ->ODR &= ~VK1024B_WR_PIN

#define VK1024B_CS_H() VK1024B_CS_GPIO_PORT ->ODR |=  VK1024B_CS_PIN
#define VK1024B_CS_L() VK1024B_CS_GPIO_PORT ->ODR &= ~VK1024B_CS_PIN

#define VK1024B_DATA_H() VK1024B_DAT_GPIO_PORT ->ODR |=  VK1024B_DAT_PIN
#define VK1024B_DATA_L() VK1024B_DAT_GPIO_PORT ->ODR &= ~VK1024B_DAT_PIN
/* Exported functions ------------------------------------------------------- */
void Vk1024B_Init(void);
void Vk1024B_DisAll(unsigned char state);
void Vk1024B_DisSegOn(unsigned char Seg,unsigned char Com);
void Vk1024B_DisSegOff(unsigned char Seg,unsigned char Com);
void Write1DataVk1024B(unsigned char Addr,unsigned char Dat);
void WritenDataVk1024B(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt);
void Vk1024B_Enter_Standby(void);
void Vk1024B_Exit_Standby(void);
void Vk1024B_TEST(void);
#endif  /*__VK1024B_H*/

/************************END OF FILE****/
