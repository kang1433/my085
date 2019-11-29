#ifndef _UART_h
#define _UART_h
#include "include.h"



#define   R_ACS_S		"ACS_" 
#define   R_DCS_S		"DCS_" 
#define   R_B3S_S		"B3S_" 
#define   R_COM_S		"KCOM_" 

#define   T_B3S_S		"B3S?_V191129\r\n" 
#define   T_ACS_S		"ACS?\r\n" 
#define   T_DCON_S		"DCON\r\n" 
#define   T_DCOFF_S		"DCOFF\r\n" 


#define B3S_OVTime			30000
#define DCS_OVTime			10000
#define UART_REC_LEN		200

#define PORT_UART 			GPIOB
#define PIN_RX				GPIO_Pin_7
#define PIN_TX				GPIO_Pin_6

#define PINSOURCE_RX		GPIO_PinSource7
#define PINSOURCE_TX		GPIO_PinSource6

#define EN_USART1_RX		1

#define AcsState	0
#define DcsState	1
#define Powbuf	2
#define B3sState	3
#define SOCbuf	4
#define Volbuf	5
#define Cell_V1	6
#define Cell_V2	7
#define Cell_V3	8
#define V_Diff	9
#define Counter	10


/******采集板状态定义******/
#define SAVE_BT_LV  20  // 过放
#define SAVE_BT_SV  21  // 过低
#define SAVE_BT_OI  22  // 放电过流

#define SAVE_FULL	1    // 充满
#define SAVE_NT_BC	26  // BAT  50°/0° 禁止充，
#define SAVE_BT_HV  27  // 过压

//#define SAVE_BT_EV	25  // 均衡压差异常大于300mV
#define SAVE_NT_BT  28  // BAT  65°/-20° NTC异常
#define SAVE_NT_FT	29  // FUSE 95°/-20° NTC异常
/******采集板状态定义******/



enum _Inquire	
{
	Iq_B3S,			
	Iq_ACS,			
	Iq_ONOFF,			
};
typedef enum _Inquire p_Inquire;
extern p_Inquire pInquire;

extern u16 UART_RX_STA;
extern char UART_RX_BUF[UART_REC_LEN];
extern int32 RX_BUF[11];


void Uart_Init(uint32_t bound);
void UART1_SendData8(u8 c);
void Uart_DB_work(void);  //调试串口 
void Inquire(void);
void ACSDCSB3S_State(void);
//char *sky_itoa(int value, char *str, unsigned int radix);
//void sky_ftoa(double value, char *str, unsigned int eps);
//void mPrintf(char * Data, ...);
#endif
