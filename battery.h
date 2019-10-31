#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "include.h"

extern u8 Vol_Counter[8];
extern u8 SOC_Counter[9];

extern u8 Restart_Num[];
extern u8 BuzzerCounter;				//蜂鸣器计数器
#define SOC_BaseTimes		2
#define Vol_BaseTimes		10
#define V12_BaseTimes		3
#define DYQ_BaseTimes		3
#define In_BaseTimes			0
#define AC_OUT_BaseTimes	1
#define CH_NorTimes			2
#define CH_ErrTimes			90
#define USB_BaseTimes		3
#define Num_BZ				10
#define NumOfRestarts		4		//重启次数
#define RestartTime			9000	//重启时间
#define MaxPow  			525
#define SUN_PWM		1
#define CH_PWM			0



/*********温度定义**********/
#define AC_P_TEMP			80
#define AC_PRe_TEMP 		60
#define FAN_ON_TEMP		45
#define FAN_OFF_TEMP		40

/*********数组位置**********/
#define   Num_DYQ			0x00	
#define   Num_V12			0x01
#define   Num_USB			0x02
#define   Num_AC_OUT		0x03
#define   Num_SUN_Ch		0x04	
#define   Num_CH_Ch			0x05	

/******电量电压定义******/
#define   Voltage_0  			9800
#define   Voltage_5  			10000
#define   Voltage_20  			10500
#define   Voltage_40  			11000
#define   Voltage_60		  	11400  //3
#define   Voltage_80		  	11800 
#define   Voltage_90		 	12300
#define   Voltage_100		 	12600
#define   DYQ_utmost	 		12250  

#define SOC_Rang	10
#define SOC_0		50
#define SOC_5		100
#define SOC_20		200
#define SOC_40		400
#define SOC_60		600
#define SOC_80		800
#define SOC_99		990
#define SOC_100		999
/******电量电压定义******/

enum _Capacity	//电量
{
	Capacity_0=0,	
	Capacity_5=5,	
	Capacity_20=20,
	Capacity_40=40,
	Capacity_60=60,
	Capacity_80=80,
	Capacity_100=100,
	Capacity_UTMOST,
};
typedef enum _Capacity P_Capacity;
extern P_Capacity Capacity;

/***************输出状态定义****************/	
enum _OUT_State	
{
	Out_None = 0,		//无输出
	Out_Normal,			//输出正常
};
typedef enum _OUT_State p_OUT_State;
extern p_OUT_State V12_OUT_State;
extern p_OUT_State DYQ_OUT_State;
extern p_OUT_State USB_OUT_State;
extern p_OUT_State AC_OUT_State;

/***************充电状态定义****************/	
enum _Charge_State	
{
	CH_None = 0,		//未开启充电
	CH_Ch,				//充电器充电
	SUN_Ch,				//太阳能充电
};
typedef enum _Charge_State p_Charge_State;
extern p_Charge_State Charge_State;
extern p_Charge_State Access_SUN;
extern p_Charge_State Access_CH;

/******************状态标志  *******************/	
typedef struct _state		
{
	u8	CH_Full_S			:1;			//充满电
	u8	DCH_S				:1;			//放电标志
	u8	CH_S				:1;			//充电标志
	
	u8	CH_Ch_S			:1;			//充电器充电开启
	u8	SUN_Ch_S			:1;			//太阳能充电开启
	u8	DYQ_S				:1;			//DYQ开启
	u8	V12_S 				:1;			//12v 开启
	u8	AC_OUT_S			:1;			//ACO开启
	u8	USB_S				:1;			

	u8	Ch_Restart_S		:1;			//Ch重启
	u8	SUN_Restart_S		:1;			//SUN重启
	u8	V12_Restart_S		:1;			//12V重启
	u8	DYQ_Restart_S		:1;			//DYQ重启
	u8	USB_Restart_S		:1;			//SUB重启

	u8	Key_S				:1;			//按键标志
	u8	Key_Press_S 		:1;			//按下计时
	u8	SW_DET_S			:1;			//开关机状态
	u8	SW_DET_CH_S		:1;			//充电开关机状态
	u8	SW_DET_Press_S 	:1;			//按下计时
	u8	SW_DET_Op_S		:1;			//操作标志

	u8	CH_LDuty_S			:1;			
	u8	SUN_LDuty_S		:1;			

	u8	AC_OV3min_S		:1;			
	u8	AC_OV15min_S		:1;	
	
	u8	Charge_P_S			:1;			//充电保护，禁止充电
	u8	B3S_Finish_S 		:1;			
	u8	DCS_Finish_S 		:1;			
	u8	Print_S				:1;			
	u8	DCH_P_S			:1;			
	u8	AC_P_S				:1;			
	u8	H_Temp_S			:1;			
	u8	ADC_Finish_S 		:1;			//ADC转换完成
}pstate;
extern pstate State;

/*************故障报警标志  ***************/	
typedef struct _BBit
{
	union
	{	struct
		{
			u8  	DYQ_LowP			:1;		//输出低电量
			u8  	V12_LowP			:1;		//输出低电量
			u8  	USB_LowP			:1;		//输出低电量
			u8  	BAT_LowP			:1;		//输出低电量
			u8  	ACO_LowP			:1;		//输出低电量
			u8 	RSVD5				:1;
			u8 	RSVD6				:1;
			u8 	RSVD7				:1;
		}BitLowP;
		u8 Byte_LowP;
	}Data_LowP;
	union
	{	struct
		{
			u8  	DYQ_Err				:1;		//输出故障
			u8  	V12_Err				:1;		//输出故障，可检测拔出自动复位
			u8  	USB_Err				:1;		//输出故障
			u8  	ACO_Err				:1;		//输出故障
			u8  	RSVD4				:1;		
			u8 	RSVD5				:1;
			u8 	RSVD6				:1;
			u8 	RSVD7				:1;
		}BitOErr;
		u8 Byte_OErr;
	}Data_OErr;
	union
	{	struct
		{
			u8  	CH_Err				:1;		//故障消失可自动复位
			u8  	CH_Verr				:1;		//充电电压不对//故障消失可自动复位
			u8  	CH_OIErr			:1;		//充电过流MOS  保护
			u8 	RSVD3				:1;
			u8  	SUN_Err				:1;		//故障消失可自动复位
			u8  	SUN_VErr			:1;		//充电电压不对//故障消失可自动复位
			u8 	SUN_OIErr			:1;		//充电过流MOS  保护
			u8 	RSVD7				:1;
		}BitIErr;
		u8 Byte_IErr;
	}Data_IErr;
	union
	{	struct
		{
			u8  	BatPro				:1;		//电池异常，禁止充放电
			u8  	ACS_Err				:1;		//逆变板通信线断开，禁止开AC
			u8  	B3Sc_Err			:1;		//采集板通信线断开，禁止充放电
			u8  	RSVD3				:1;		
			u8  	RSVD4				:1;		
			u8 	RSVD5				:1;
			u8 	RSVD6				:1;
			u8 	RSVD7				:1;
		}BitBat;
		u8 Byte_Bat;
	}Data_Bat;
}pBBit;
extern pBBit BuzzerBit;
extern pBBit DisplayBit;
/**********************end**************************/
void Printfstatus(void);
void Capacity_Init(void);
void Check_Capacity_Sta(void);
void Check_Charge_Sta(void);
void Operate_CH_Ch(void);
void Check_Temp(void);
void Check_Sta(void);
void Operate_De(void);

#endif
