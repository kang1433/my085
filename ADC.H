



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


/*********充电电压阀值定义*********/
#define   Diode_V				400					//单位mV
#define   N_V_Charger		(9000- Diode_V)		//单位mV
#define   C_V_Charger		(12400-Diode_V)
#define   P_V_Charger		(13100-Diode_V)	

#define   N_V_SUN			(9000- Diode_V)		//单位mV
#define   C_V_SUN			(12400-Diode_V)
#define   P_V_SUN			(25000-Diode_V)	
/*********充电电流阀值定义*********/
#define   Zero_I				200		//ma
#define   SUN_I_Least			800		//ma
#define   SUN_I_Less			2500	
#define   SUN_I_Little			4000	
#define   SUN_I_Nor 			5100	
#define   SUN_I_Pro			5700

#define   CH_I_Least			800		//ma
#define   CH_I_Less			4000	
#define   CH_I_Little			9000	
#define   CH_I_Nor 			10100	
#define   CH_I_Pro			10500

/*********DC电流阀值定义*********/
#define   B_I_12V		 		50		//ma
#define   C_I_12V		 		200		
#define   P_I_12V		 		2800		

#define   B_I_DYQ		 	50		//ma
#define   C_I_DYQ		 	200		
#define   P_I_DYQ		 	7900

#ifdef  old
#define DYQRATIO			140//135
#else
#define DYQRATIO			136
#endif


void ADC1_Init(void);
signed short CalculateTemperature(float R);
void GetTemperature(void);
void ADC_Filter(void);
void FILTER_UI(u16 *BUFFER,u8 num);

#endif
