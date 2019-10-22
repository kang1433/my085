/**************************************************************/
/* Sysclock.c */
/* Copyright (c) 2018, Mr Kang. */
/**************************************************************/

#include "sysclock.h"

u8 	BuzzerTime = 0;			//鸣叫时间
u8 	Key_PressTime = 0;		//按键时间
u16 	SW_DET_PressTime = 0;	//总开关按下时间
u16	UART_RX_Time = 0;      	//串口接收时间
u16	B3S_RX_Time = B3S_OVTime;      	//电池数据接收时间
u16	DCS_RX_Time = DCS_OVTime;      	//逆变数据接收时间

u32	Open_Time =0; 			//开机时间  
u32	LowP_Time =0; 			//低电量时间  
u16	CH_LowDuty_Time =0; 		//低占空比时间  
u16	SUN_LowDuty_Time =0; 		//低占空比时间  

u16 ResTime[6];		//重启时间
u16 Uptime[6];		//正常运行时间
u32 ACOVTime[2];

void SysTickInit(void)
{
	//* SystemFrequency / 1000    1ms中断一次
	// * SystemFrequency / 100000	 10us中断一次
	// * SystemFrequency / 1000000 1us中断一次
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0库版本  1ms中断一次
	{ 
	//	* Capture error *
		while (1);
	}
	// 关闭滴答定时器  
	//SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

//void Delay_us(uint32_t nTime)
//{
//	SysTick->LOAD=48*nTime; // 重装值；48个时钟周期=1us
//	SysTick->CTRL=0x00000005;// 第0位：使能时钟 第3位：使用内部时钟
//	//SysTick->VAL=0X00;              //  初值
//	SysTick->LOAD=48*nTime;    //  重装值；48个时钟周期=1us
//	SysTick->CTRL=0x00000005;//  第0位：使能时钟   第3位：使用内部时钟
//	while(!(SysTick->CTRL&0x00010000));//计时
//	SysTick->CTRL|=0x00000004;// 第1位：产生异常请求
//}

//void Delay_ms(u32 i)
//{
//	u32 temp;
//	SysTick->LOAD=6000*i;      //设置重装数值
//	SysTick->CTRL=0X01;        //使能，减到零是无动作，采用外部时钟源
//	SysTick->VAL=0;            //清零计数器
//	do
//	{
//		temp=SysTick->CTRL;       //读取当前倒计数值
//	}
//	while((temp&0x01)&&(!(temp&(1<<16))));    //等待时间到达
//	SysTick->CTRL=0;    //关闭计数器
//	SysTick->VAL=0;        //清空计数器
//}

u32 u32timerms=0;

void SysTick_Update(void)
{
	u32timerms++;
	if((u32timerms&0xFFFF)==0xFFFF){
		//此处对所有定时进行处理
	}
}

void SysTick_Correct(u32 timeMs)
{
	u32timerms +=timeMs;
}

u32 SysTimeMs(void)
{
	u32 t1,t2;
	do{
		t1=u32timerms;
		t2=u32timerms;
	}while(t1!=t2);
	return t1;
}

u16 u16SysTimeMs(void)
{
	u32 t1,t2;
	do{
		t1=u32timerms;
		t2=u32timerms;
	}while(t1!=t2);
	return (u16)t1;
}

void DelayMs(u16 ms)
{
	u32 time = SysTimeMs();
	while(!SysTimeMs());
	while((SysTimeMs() - time)<ms);
}


void SYSCLKConfig_STOP(void)
{ 
	/* After wake-up from STOP reconfigure the system clock */
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
	{}

	/* Enable PLL */
	RCC_PLLCmd(ENABLE);

	/* Wait till PLL is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{}

	/* Select PLL as system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	/* Wait till PLL is used as system clock source */
	while (RCC_GetSYSCLKSource() != 0x08)
	{}
}




void SysTick_Handler(void)	//滴答定时器1MS
{
	SysTick_Update();
	task_TickCut();
	if(Open_Time < 30000)
		Open_Time++;
	else if(Open_Time < CLOSETIME)
	{
		if((!State.DCH_S) && (!State.CH_S) && (!Access_SUN) && (!Access_CH))
			Open_Time++;
		else
			Open_Time = 30000;
	}
	if((DisplayBit.Data_LowP.Byte_LowP & 0x0f) && (LowP_Time < LOWPCLOSETIME))
		LowP_Time++;
	else 
		LowP_Time = 0;
	
	if((State.CH_LDuty_S) && (CH_LowDuty_Time < LDUTYTIME))
		CH_LowDuty_Time++;
	if((State.SUN_LDuty_S) && (SUN_LowDuty_Time < LDUTYTIME))
		SUN_LowDuty_Time++;
	
	/*按键按下时间计时*/
	if((State.Key_Press_S) && (Key_PressTime < 100))
		Key_PressTime++;
	if((State.SW_DET_Press_S) && (SW_DET_PressTime < 2100))
		SW_DET_PressTime++;
	
	/*异常后重启等待时间计时*/
	if((State.Ch_Restart_S)&&(ResTime[Ch_Time]<RestartTime))
		ResTime[Ch_Time]++;
	if((State.SUN_Restart_S)&&(ResTime[SUN_Time]<RestartTime))
		ResTime[SUN_Time]++;
	if((State.V12_Restart_S)&&(ResTime[V12_Time]<RestartTime))
		ResTime[V12_Time]++;
	if((State.USB_Restart_S)&&(ResTime[USB_Time]<RestartTime))
		ResTime[USB_Time]++;
	if((State.DYQ_Restart_S)&&(ResTime[DYQ_Time]<RestartTime))
		ResTime[DYQ_Time]++;
	
	/*开启后计时*/
	if((State.V12_S) && (Uptime[V12_Time] < OpDuration))
		Uptime[V12_Time]++;
	if((State.DYQ_S) && (Uptime[DYQ_Time] < OpDuration))
		Uptime[DYQ_Time]++;
	if((State.USB_S) && (Uptime[USB_Time] < OpDuration))
		Uptime[USB_Time]++;
	if((State.AC_OUT_S) && (Uptime[AC_Time] < OpDuration))
		Uptime[AC_Time]++;
	if((Charge_State == CH_Ch) && (Uptime[Ch_Time] < OpDuration))
		Uptime[Ch_Time]++;
	if((Charge_State == SUN_Ch) && (Uptime[SUN_Time] < OpDuration))
		Uptime[SUN_Time]++;
	
	if((State.AC_OV3min_S) && (ACOVTime[0] < OneHour))
		ACOVTime[0]++;
	if((State.AC_OV15min_S) && (ACOVTime[1] < OneHour))
		ACOVTime[1]++;
	
	if(UART_RX_Time > 0)
	{
		UART_RX_Time--;
		if(UART_RX_Time == 0)
		{
			if(UART_RX_STA < 15)
				UART_RX_STA = 0;
			else
				UART_RX_STA |= 0x8000;	//接收完成了 			
		}
	}
	if(B3S_RX_Time > 0)
		B3S_RX_Time--;
	else
	{
		if(!DisplayBit.Data_Bat.BitBat.B3Sc_Err)
		{
			BuzzerBit.Data_Bat.BitBat.B3Sc_Err = 1;
			DisplayBit.Data_Bat.BitBat.B3Sc_Err = 1;
		}
	}
	if(DCS_RX_Time > 0)
		DCS_RX_Time--;
	else
	{
		if(!DisplayBit.Data_Bat.BitBat.ACS_Err)
		{
			BuzzerBit.Data_Bat.BitBat.ACS_Err = 1;
			DisplayBit.Data_Bat.BitBat.ACS_Err = 1;
			AC_OUT_Op(0);
		}
	}
}


