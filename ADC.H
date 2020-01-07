



#ifndef _ADC_h
#define _ADC_h


#include "include.h"

#define ADC1_DR_Address		0x40012440
#define AD_Count   			50			//AD滤波采样次数
#define AD_ChNUM			8			//AD通道数

extern u16 AD_Value[AD_ChNUM];
extern int32 AD_Data[AD_ChNUM];

#define TempSize 			131 //定义表长度 
#define TempTop				130
#define TempMid				65

/******************ADC通道选择定义*********************/
#define   ADC_I_12V		 	0x00	//12V电流检测
#define   ADC_I_Charge		0x02	//充电电流检测	
#define   ADC_NTC			0x03	//NTC检测	
#define   ADC_V_Bat			0x04	//电池电压检测
#define   ADC_V_Charger		0x05	//充电检测	
#define   ADC_V_SUN			0x06	//充电检测	
#define   ADC_V_DYQ		 	0x08	//点烟口前级电压检测	
#define   ADC_I_DYQ		 	0x09	//点烟口后级电流检测	

#define   AD_I_12V		 	0x00	
#define   AD_I_Charge		0x01	
#define   AD_NTC		 		0x02	
#define   AD_V_Bat			0x03	
#define   AD_V_Charger		0x04	
#define   AD_V_SUN			0x05
#define   AD_V_DYQ		 	0x06	
#define   AD_I_DYQ		 	0x07	


#define DYQRATIO			132.33 	// 136



void ADC1_Init(void);
int32 CalculateTemperature(float R);
void GetTemperature(void);
void ADC_Filter(void);
void FILTER_UI(u16 *BUFFER,u8 num);

#endif
