/**************************************************************/
/* Sysclock.c */
/* Copyright (c) 2018, Mr Kang. */
/**************************************************************/

#include "sysclock.h"

u8 	BuzzerTime = 0;			//����ʱ��
u8 	Key_PressTime = 0;		//����ʱ��
u16 	SW_DET_PressTime = 0;	//�ܿ��ذ���ʱ��
u16	UART_RX_Time = 0;      	//���ڽ���ʱ��
u16	B3S_RX_Time = B3S_OVTime;      	//������ݽ���ʱ��
u16	DCS_RX_Time = DCS_OVTime;      	//������ݽ���ʱ��

u32	Open_Time =0; 			//����ʱ��  
u32	LowP_Time =0; 			//�͵���ʱ��  
u32	DYQ_INIT_Time =0; 			//
u16	SUN_LowDuty_Time =0; 		//��ռ�ձ�ʱ��  

u16 ResTime[6];		//����ʱ��
u16 Uptime[6];		//��������ʱ��
u32 ACOVTime[2];

void SysTickInit(void)
{
	//* SystemFrequency / 1000    1ms�ж�һ��
	// * SystemFrequency / 100000	 10us�ж�һ��
	// * SystemFrequency / 1000000 1us�ж�һ��
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0��汾  1ms�ж�һ��
	{ 
	//	* Capture error *
		while (1);
	}
	// �رյδ�ʱ��  
	//SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	// ʹ�ܵδ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

//void Delay_us(uint32_t nTime)
//{
//	SysTick->LOAD=48*nTime; // ��װֵ��48��ʱ������=1us
//	SysTick->CTRL=0x00000005;// ��0λ��ʹ��ʱ�� ��3λ��ʹ���ڲ�ʱ��
//	//SysTick->VAL=0X00;              //  ��ֵ
//	SysTick->LOAD=48*nTime;    //  ��װֵ��48��ʱ������=1us
//	SysTick->CTRL=0x00000005;//  ��0λ��ʹ��ʱ��   ��3λ��ʹ���ڲ�ʱ��
//	while(!(SysTick->CTRL&0x00010000));//��ʱ
//	SysTick->CTRL|=0x00000004;// ��1λ�������쳣����
//}

//void Delay_ms(u32 i)
//{
//	u32 temp;
//	SysTick->LOAD=6000*i;      //������װ��ֵ
//	SysTick->CTRL=0X01;        //ʹ�ܣ����������޶����������ⲿʱ��Դ
//	SysTick->VAL=0;            //���������
//	do
//	{
//		temp=SysTick->CTRL;       //��ȡ��ǰ������ֵ
//	}
//	while((temp&0x01)&&(!(temp&(1<<16))));    //�ȴ�ʱ�䵽��
//	SysTick->CTRL=0;    //�رռ�����
//	SysTick->VAL=0;        //��ռ�����
//}

u32 u32timerms=0;

void SysTick_Update(void)
{
	u32timerms++;
	if((u32timerms&0xFFFF)==0xFFFF){
		//�˴������ж�ʱ���д���
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




void SysTick_Handler(void)	//�δ�ʱ��1MS
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
	if(LowP_Time < LOWPCLOSETIME)
	{
		if(DisplayBit.Data_LowP.Byte_LowP & 0x0f)
			LowP_Time++;
		else 
			LowP_Time = 0;
	}	
	if((State.SUN_LDuty_S) && (SUN_LowDuty_Time < SUNLDUTYTIME))
		SUN_LowDuty_Time++;
	
	/*��������ʱ���ʱ*/
	if((State.Key_Press_S) && (Key_PressTime < 100))
		Key_PressTime++;
	if((State.SW_DET_Press_S) && (SW_DET_PressTime < 2100))
		SW_DET_PressTime++;
	
	/*�쳣�������ȴ�ʱ���ʱ*/
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
	
	/*�������ʱ*/
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
				UART_RX_STA |= 0x8000;	//��������� 			
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


