
#include "battery.h"


u8 Vol_Counter[8]={0};
u8 SOC_Counter[9]={0};
u8 DYQ_Counter[7]={0};
u8 V12_Counter[4]={0};
u8 USB_Counter[4]={0};
u8 AC_OUT_Counter[6]={0};
u8 Charger_Counter[4]={0};
u8 SUN_Counter[4]={0};
u8 CheckInputCont[3]={0};
u8 Duty_Counter[3]={0};
u8 Restart_Num[6]={0};
u8 BuzzerCounter = 0;				//蜂鸣器计数器

P_Capacity Capacity;
pstate State;
p_OUT_State V12_OUT_State;
p_OUT_State DYQ_OUT_State;
p_OUT_State USB_OUT_State;
p_OUT_State AC_OUT_State;
p_Charge_State Charge_State;
p_Charge_State Access_SUN;
p_Charge_State Access_CH;
pBBit BuzzerBit;
pBBit DisplayBit;

u16 Charge_Dutycycle=START_Dutycycle;
u16 P_I_DYQ_Vau;
u8 duty;

void Printfstatus(void)
{
	IFPrintf(State.CH_Full_S);
	IFPrintf(State.DCH_S);
	IFPrintf(State.CH_S);
	IFPrintf(State.CH_Ch_S);
	IFPrintf(State.SUN_Ch_S);
	IFPrintf(State.DYQ_S);
	IFPrintf(State.V12_S);
	IFPrintf(State.AC_OUT_S);
	IFPrintf(State.USB_S);
	IFPrintf(State.Ch_Restart_S);
	IFPrintf(State.SUN_Restart_S);
	IFPrintf(State.V12_Restart_S);
	IFPrintf(State.DYQ_Restart_S);
	IFPrintf(State.USB_Restart_S);
	IFPrintf(State.Key_S);
	IFPrintf(State.Key_Press_S);
//	IFPrintf(State.SW_DET_S);
	IFPrintf(State.SW_DET_CH_S);
	IFPrintf(State.SW_DET_Press_S);
	IFPrintf(State.SW_DET_Op_S);
	IFPrintf(State.CH_LDuty_S);
	IFPrintf(State.SUN_LDuty_S);
	IFPrintf(State.AC_OV3min_S);
	IFPrintf(State.AC_OV15min_S);
	IFPrintf(State.Charge_P_S);
	IFPrintf(State.B3S_Finish_S);
	IFPrintf(State.DCS_Finish_S);
	IFPrintf(State.DCH_P_S);
	IFPrintf(State.AC_P_S);
	IFPrintf(State.H_Temp_S);
	IFPrintf(Access_SUN);
	IFPrintf(Access_CH);
	
	IFPrintf(BuzzerBit.Data_OErr.BitOErr.DYQ_Err);
	IFPrintf(BuzzerBit.Data_OErr.BitOErr.V12_Err);
	IFPrintf(BuzzerBit.Data_OErr.BitOErr.ACO_Err);
	IFPrintf(BuzzerBit.Data_OErr.BitOErr.USB_Err);
	IFPrintf(BuzzerBit.Data_Bat.BitBat.BatPro);
	IFPrintf(BuzzerBit.Data_Bat.BitBat.ACS_Err);
	IFPrintf(BuzzerBit.Data_Bat.BitBat.B3Sc_Err);
	IFPrintf(BuzzerBit.Data_LowP.Byte_LowP);
	
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.CH_Err);
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.CH_Verr);
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.CH_OIErr);
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.CH_TErr);
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.SUN_Err);
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.SUN_VErr);
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.SUN_OIErr);
	IFPrintf(BuzzerBit.Data_IErr.BitIErr.SUN_TErr);

	IFPrintf(DisplayBit.Data_OErr.BitOErr.DYQ_Err);
	IFPrintf(DisplayBit.Data_OErr.BitOErr.V12_Err);
	IFPrintf(DisplayBit.Data_OErr.BitOErr.ACO_Err);
	IFPrintf(DisplayBit.Data_OErr.BitOErr.USB_Err);
	IFPrintf(DisplayBit.Data_Bat.BitBat.BatPro);
	IFPrintf(DisplayBit.Data_Bat.BitBat.ACS_Err);
	IFPrintf(DisplayBit.Data_Bat.BitBat.B3Sc_Err);
	IFPrintf(DisplayBit.Data_LowP.Byte_LowP);
	
	IFPrintf(DisplayBit.Data_IErr.BitIErr.CH_Err);
	IFPrintf(DisplayBit.Data_IErr.BitIErr.CH_Verr);
	IFPrintf(DisplayBit.Data_IErr.BitIErr.CH_OIErr);
	IFPrintf(DisplayBit.Data_IErr.BitIErr.CH_TErr);
	IFPrintf(DisplayBit.Data_IErr.BitIErr.SUN_Err);
	IFPrintf(DisplayBit.Data_IErr.BitIErr.SUN_VErr);
	IFPrintf(DisplayBit.Data_IErr.BitIErr.SUN_OIErr);
	IFPrintf(DisplayBit.Data_IErr.BitIErr.SUN_TErr);
	printf("\r\n");
	for(u8 i=0;i<AD_ChNUM;i++)
		printf(" AD%d=%d ",i,AD_Data[i]);
	printf("\r\n");
	for(u8 j=0;j<10;j++)
		printf(" RX%d=%d ",j,RX_BUF[j]);
	printf("\r\n *T3=%d",ACOVTime[0]);
	printf(" *T15=%d",ACOVTime[1]);
	printf(" *OT=%d",Open_Time);
	printf(" *LT=%d",LowP_Time);
	printf(" *DC=%d",Charge_Dutycycle);
}

void Clear_CH_ERR(void)
{
	if(Uptime[Ch_Time] >= OpDuration)
	{
		Restart_Num[Num_CH_Ch] = 0;
		BuzzerBit.Data_IErr.BitIErr.CH_Err = 0;
		DisplayBit.Data_IErr.BitIErr.CH_Err = 0;
	}
}

void Clear_SUN_ERR(void)
{
	if(Uptime[SUN_Time] >= OpDuration)
	{
		Restart_Num[Num_CH_Ch] = 0;
		BuzzerBit.Data_IErr.BitIErr.SUN_Err = 0;
		DisplayBit.Data_IErr.BitIErr.SUN_Err = 0;
	}
}

void Clear_V12_ERR(void)
{
	if(Uptime[V12_Time] >= OpDuration)
	{
		Restart_Num[Num_V12] = 0;
		BuzzerBit.Data_OErr.BitOErr.V12_Err = 0;
		DisplayBit.Data_OErr.BitOErr.V12_Err = 0;
	}
}

void Clear_DYQ_ERR(void)
{
	if(Uptime[DYQ_Time] >= OpDuration)
	{
		Restart_Num[Num_DYQ] = 0;
		BuzzerBit.Data_OErr.BitOErr.DYQ_Err = 0;
		DisplayBit.Data_OErr.BitOErr.DYQ_Err = 0;
	}
}

void Clear_USB_ERR(void)
{
	if(Uptime[USB_Time] >= OpDuration)
	{
		Restart_Num[Num_USB] = 0;
		BuzzerBit.Data_OErr.BitOErr.USB_Err = 0;
		DisplayBit.Data_OErr.BitOErr.USB_Err = 0;
		BuzzerBit.Data_OErr.BitOErr.USB_Err2 = 0;
		DisplayBit.Data_OErr.BitOErr.USB_Err2 = 0;
	}
}

void Clear_ACOUT_ERR(void)
{
	if(Uptime[AC_Time] >= OpDuration)
	{
		BuzzerBit.Data_OErr.BitOErr.ACO_Err = 0;
		DisplayBit.Data_OErr.BitOErr.ACO_Err = 0;
	}
}

void SET_DYQ_ERR(void)
{
	if(!DisplayBit.Data_OErr.BitOErr.DYQ_Err)
	{
		BuzzerBit.Data_OErr.BitOErr.DYQ_Err = 1;
		DisplayBit.Data_OErr.BitOErr.DYQ_Err = 1;
	}
}

void SET_ACOUT_ERR(void)
{
	Restart_Num[Num_AC_OUT]++;
	if(!DisplayBit.Data_OErr.BitOErr.ACO_Err)
	{
		BuzzerBit.Data_OErr.BitOErr.ACO_Err = 1;
		DisplayBit.Data_OErr.BitOErr.ACO_Err = 1;
	}
}

void V12_I_Sta(u8 n)
{
	if(State.V12_S)
	{
		if(AD_Data[AD_I_12V] >= (n * P_I_12V))
		{
			K_memset(0, V12_Counter,sizeof(V12_Counter));
			if(V12_Counter[0] > (V12_BaseTimes))
			{
				V12_Counter[0] = 0;
				Restart_Num[Num_V12]++;
				V12_Op(0);
				if(!DisplayBit.Data_OErr.BitOErr.V12_Err)
				{
					BuzzerBit.Data_OErr.BitOErr.V12_Err = 1;
					DisplayBit.Data_OErr.BitOErr.V12_Err = 1;
				}
			}
		}
		else if((AD_Data[AD_I_12V] < (n * P_I_12V)) && (AD_Data[AD_I_12V] >= C_I_12V)) 
		{
			K_memset(1, V12_Counter,sizeof(V12_Counter));
			if(V12_Counter[1] > (V12_BaseTimes))
			{
				V12_Counter[1] = 0;
				V12_OUT_State = Out_Normal;
				Clear_V12_ERR();
			}
		}
		else	if((AD_Data[AD_I_12V] < C_I_12V) && (AD_Data[AD_I_12V] >= (C_I_12V - B_I_12V)))
		{
			K_memset(2, V12_Counter,sizeof(V12_Counter));
			if(V12_Counter[2] > (V12_BaseTimes))
			{
				V12_Counter[2] = 0;
				Clear_V12_ERR();
			}
		}
		else 
		{	
			K_memset(3, V12_Counter,sizeof(V12_Counter));
			if(V12_Counter[3] > (V12_BaseTimes))
			{
				V12_Counter[3] = 0;
				V12_OUT_State = Out_None;
				Clear_V12_ERR();
			}
		}
	}
}



void Capacity_InVol(void)
{
	if(AD_Data[AD_V_Bat] <= Voltage_5) 
	{
		K_memset(0, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[0] > Vol_BaseTimes)
		{
			Vol_Counter[0] = 0;
			State.CH_Full_S = 0;
			Capacity=Capacity_0;
			if((!DisplayBit.Data_LowP.BitLowP.BAT_LowP)&&(Charge_State == CH_None))
			{
				BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 1;
				DisplayBit.Data_LowP.BitLowP.BAT_LowP = 1;
			}
		}
	}
	else if(AD_Data[AD_V_Bat] <= Voltage_20) 
	{
		K_memset(1, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[1] > Vol_BaseTimes)
		{
			Vol_Counter[1] = 0;
			State.CH_Full_S = 0;
			if(!State.DCH_P_S)//未DC带载到电量低
			{
				BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
				DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				Capacity=Capacity_5;
			}
		}
	}
	else if(AD_Data[AD_V_Bat] <= Voltage_40) 
	{
		K_memset(2, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[2] > Vol_BaseTimes)
		{
			Vol_Counter[2] = 0;
			State.CH_Full_S = 0;
			if(!State.DCH_P_S)//未DC带载到电量低
			{
				BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
				DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				Capacity=Capacity_20;
			}
		}
	}
	else if(AD_Data[AD_V_Bat] <= Voltage_60) 
	{
		K_memset(3, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[3] > Vol_BaseTimes)
		{
			Vol_Counter[3] = 0;
			State.CH_Full_S = 0;
			if((!State.DCH_P_S) && (!State.AC_P_S))	//未DC带载到电量低
			{
				BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
				DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				Capacity=Capacity_40;
			}
		}
	}
	else if(AD_Data[AD_V_Bat] <= Voltage_80) 
	{
		K_memset(4, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[4] > Vol_BaseTimes)
		{
			Vol_Counter[4] = 0;
			State.CH_Full_S = 0;
			if((!State.DCH_P_S) && (!State.AC_P_S))
			{
				BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
				DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				Capacity=Capacity_60;
			}
		}
	}	
	else if(AD_Data[AD_V_Bat] < Voltage_90) 
	{
		K_memset(5, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[5] > Vol_BaseTimes)
		{
			Vol_Counter[5] = 0;
			State.CH_Full_S = 0;
			BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
			DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
			if(!State.AC_P_S)
				Capacity=Capacity_80;
		}
	}
	else if(AD_Data[AD_V_Bat] < Voltage_100) 
	{
		K_memset(6, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[6] > Vol_BaseTimes)
		{
			Vol_Counter[6] = 0;
			BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
			DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
			Capacity=Capacity_100;
		}
	}
	else
	{
		K_memset(7, Vol_Counter, sizeof(Vol_Counter));
		if(Vol_Counter[7] > Vol_BaseTimes)
		{
			Vol_Counter[7] = 0;
			BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
			DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
			Capacity=Capacity_100;
		}
	}
}

void Capacity_Init(void)
{	
	if(RX_BUF[Volbuf] == 0)//未接受到采集板信息，使用电压判断电量
		Capacity_InVol();
	else	//使用采集板SOC判断电量
	{
		if(State.B3S_Finish_S)
		{
			State.B3S_Finish_S= 0;
			if((RX_BUF[SOCbuf] < 0) || (RX_BUF[SOCbuf] > 1001))
				return;
			if(RX_BUF[SOCbuf] <= (SOC_5-SOC_Rang)) 
			{
				K_memset(0, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[0] > SOC_BaseTimes)
				{
					SOC_Counter[0] = 0;
					Capacity=Capacity_0;
					State.CH_Full_S = 0;
					if((!DisplayBit.Data_LowP.BitLowP.BAT_LowP)&&(Charge_State == CH_None))
					{
						BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 1;
						DisplayBit.Data_LowP.BitLowP.BAT_LowP = 1;
					}
				}
			}
			else if(RX_BUF[SOCbuf] <= (SOC_20-SOC_Rang)) 
			{
				K_memset(1, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[1] > SOC_BaseTimes)
				{
					SOC_Counter[1] = 0;
					Capacity=Capacity_5;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if(RX_BUF[SOCbuf] <= (SOC_40-SOC_Rang)) 
			{
				K_memset(2, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[2] > SOC_BaseTimes)
				{
					SOC_Counter[2] = 0;
					Capacity=Capacity_20;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if(RX_BUF[SOCbuf] <= (SOC_60-SOC_Rang)) 
			{
				K_memset(3, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[3] > SOC_BaseTimes)
				{
					SOC_Counter[3] = 0;
					Capacity=Capacity_40;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if(RX_BUF[SOCbuf] <= (SOC_80-SOC_Rang)) 
			{
				K_memset(4, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[4] > SOC_BaseTimes)
				{
					SOC_Counter[4] = 0;
					Capacity=Capacity_60;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if(RX_BUF[SOCbuf] <= SOC_90) 
			{
				K_memset(5, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[5] > SOC_BaseTimes)
				{
					SOC_Counter[5] = 0;
					Capacity=Capacity_80;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else 
			{
				K_memset(6, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[6] > SOC_BaseTimes)
				{
					SOC_Counter[6] = 0;
					Capacity=Capacity_100;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
		}
	}
}


void Check_Capacity_Sta(void)
{	
	u16 max,min;
	if(AD_Data[AD_V_Bat] >= (Voltage_100+50)) 
	{
		K_memset(8, SOC_Counter,sizeof(SOC_Counter));
		if(SOC_Counter[8] > (2*SOC_BaseTimes))
		{
			SOC_Counter[8] = 0;
			State.CH_Full_S = 1;
		}
	}
	if(DisplayBit.Data_Bat.BitBat.B3Sc_Err)//未接受到采集板信息，使用电压判断电量
	{
		K_memset(7, SOC_Counter,sizeof(SOC_Counter));
		if(SOC_Counter[7] > (10*SOC_BaseTimes))
		{
			SOC_Counter[7] = 0;
			Capacity_InVol();
		}
	}
	else	//使用采集板SOC判断电量
	{
		if(State.B3S_Finish_S)
		{
			State.B3S_Finish_S = 0;
			if((RX_BUF[SOCbuf] < 0) || (RX_BUF[SOCbuf] > 1001))
				return;
			if(RX_BUF[SOCbuf] <= SOC_95)
				State.CH_Full_S = 0;
			if(RX_BUF[SOCbuf] <= SOC_0)
			{
				K_memset(0, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[0] > (3*SOC_BaseTimes))
				{
					SOC_Counter[0] = 0;
					Capacity=Capacity_0;
					State.CH_Full_S = 0;
					if((!DisplayBit.Data_LowP.BitLowP.BAT_LowP)&&(Charge_State == CH_None))
					{
						BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 1;
						DisplayBit.Data_LowP.BitLowP.BAT_LowP = 1;
					}
				}
			}
			else if((RX_BUF[SOCbuf] < (SOC_5+SOC_Rang)) 
				&&(RX_BUF[SOCbuf] > (SOC_5-SOC_Rang)))
			{
				K_memset(1, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[1] > SOC_BaseTimes)
				{
					SOC_Counter[1] = 0;
					Capacity=Capacity_5;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if((RX_BUF[SOCbuf] < (SOC_20+SOC_Rang)) 
				&&(RX_BUF[SOCbuf] > (SOC_20-SOC_Rang)))
			{
				K_memset(2, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[2] > SOC_BaseTimes)
				{
					SOC_Counter[2] = 0;
					Capacity=Capacity_20;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if((RX_BUF[SOCbuf] < (SOC_40+SOC_Rang)) 
				&&(RX_BUF[SOCbuf] > (SOC_40-SOC_Rang)))
			{
				K_memset(3, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[3] > SOC_BaseTimes)
				{
					SOC_Counter[3] = 0;
					Capacity=Capacity_40;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if((RX_BUF[SOCbuf] < (SOC_60+SOC_Rang)) 
				&&(RX_BUF[SOCbuf] > (SOC_60-SOC_Rang)))
			{
				K_memset(4, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[4] > SOC_BaseTimes)
				{
					SOC_Counter[4] = 0;
					Capacity=Capacity_60;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if((RX_BUF[SOCbuf] < (SOC_80+SOC_Rang)) 
				&&(RX_BUF[SOCbuf] > (SOC_80-SOC_Rang)))
			{
				K_memset(5, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[5] > SOC_BaseTimes)
				{
					SOC_Counter[5] = 0;
					Capacity=Capacity_80;
					State.CH_Full_S = 0;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
				}
			}
			else if(RX_BUF[SOCbuf] >= SOC_100)
			{
				K_memset(6, SOC_Counter,sizeof(SOC_Counter));
				if(SOC_Counter[6] > SOC_BaseTimes)
				{
					SOC_Counter[6] = 0;
					Capacity=Capacity_100;
					BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.BAT_LowP = 0;
					if(RX_BUF[B3sState]==SAVE_FULL) 
					{
						max=(RX_BUF[Cell_V1]>RX_BUF[Cell_V2]) ? RX_BUF[Cell_V1] : RX_BUF[Cell_V2];
						max=(RX_BUF[Cell_V3]>max) ? RX_BUF[Cell_V3] : max;
						min=(RX_BUF[Cell_V1]<RX_BUF[Cell_V2]) ? RX_BUF[Cell_V1] : RX_BUF[Cell_V2];
						min=(RX_BUF[Cell_V3]<min) ? RX_BUF[Cell_V3] : min;
						RX_BUF[V_Diff] = max - min;
						if(RX_BUF[V_Diff] < 300)//压差正常，置位充满
							State.CH_Full_S = 1;
					}
				}
			}
		}
	}
}

void Check_DYQ_Sta(void)
{
	if(State.DYQ_S)
	{
		P_I_DYQ_Vau = P_I_DYQ;
		if(AD_Data[AD_V_Bat] <= 9900)
			P_I_DYQ_Vau = P_I_DYQ;
		else if((AD_Data[AD_V_Bat] > 9900) && (AD_Data[AD_V_Bat] <= 11400))
			P_I_DYQ_Vau -= 0.39*(AD_Data[AD_V_Bat] - 9900);
		else if((AD_Data[AD_V_Bat] > 11400) && (AD_Data[AD_V_Bat] <= 11900))
		{
			P_I_DYQ_Vau -=585;
			P_I_DYQ_Vau += 0.66*(AD_Data[AD_V_Bat] - 11400);
		}
		else if((AD_Data[AD_V_Bat] > 11900) && (AD_Data[AD_V_Bat] <= 12200))
		{
			P_I_DYQ_Vau -=255;
			P_I_DYQ_Vau += 2.25*(AD_Data[AD_V_Bat] - 11900);
		}
		else
			P_I_DYQ_Vau += 420;
		
		if((AD_Data[AD_V_Bat] > (Voltage_100+100)) || ((AD_Data[AD_V_DYQ] > DYQ_utmost) && (Uptime[DYQ_Time] > 1000)))
		{
			K_memset(0, DYQ_Counter,sizeof(DYQ_Counter));
			if(DYQ_Counter[0] > DYQ_BaseTimes)
			{
				DYQ_Counter[0] = 0;
				DYQ_Op(0);
				SET_DYQ_ERR();
			}
		}
		else if((GET_DYQ_PG == 0)&&(Uptime[DYQ_Time] >= OpDuration))//点烟器过流或者故障
		{
			K_memset(1, DYQ_Counter,sizeof(DYQ_Counter));
			if(DYQ_Counter[1] > DYQ_BaseTimes)
			{
				DYQ_Counter[1] = 0;
				DYQ_Op(0);
				Restart_Num[Num_DYQ]++;
				SET_DYQ_ERR();
			}
		}
		else if(AD_Data[AD_I_DYQ] >= P_I_DYQ_Vau)//点烟器过流或者故障
		{
			K_memset(2, DYQ_Counter,sizeof(DYQ_Counter));
			if(DYQ_Counter[2] > DYQ_BaseTimes)
			{
				DYQ_Counter[2] = 2;
				DYQ_Op(0);
				Restart_Num[Num_DYQ]++;
				SET_DYQ_ERR();
			}
		}
		else	if((AD_Data[AD_I_DYQ] < P_I_DYQ_Vau) && (AD_Data[AD_I_DYQ] >= (P_I_DYQ_Vau - 1500)))//点烟器输出电流大
		{
			K_memset(3, DYQ_Counter,sizeof(DYQ_Counter));
			if(DYQ_Counter[3] > (DYQ_BaseTimes))
			{
				DYQ_Counter[3] = 0;
				DYQ_OUT_State = Out_Normal;
				State.H_DYQIFAN_S = 1;
				Clear_DYQ_ERR();
			}
		}
		else	if((AD_Data[AD_I_DYQ] < (P_I_DYQ_Vau - 1500)) && (AD_Data[AD_I_DYQ] >= C_I_DYQ))		//点烟器有输出
		{
			K_memset(4, DYQ_Counter,sizeof(DYQ_Counter));
			if(DYQ_Counter[4] > (DYQ_BaseTimes))
			{
				DYQ_Counter[4] = 0;
				DYQ_OUT_State = Out_Normal;
				State.H_DYQIFAN_S = 0;
				Clear_DYQ_ERR();
			}
		}
		else if((AD_Data[AD_I_DYQ] < C_I_DYQ) && (AD_Data[AD_I_DYQ] >= (C_I_DYQ - B_I_DYQ)))
		{
			K_memset(5, DYQ_Counter,sizeof(DYQ_Counter));
			if(DYQ_Counter[5] > (DYQ_BaseTimes))
			{
				DYQ_Counter[5] = 0;
				State.H_DYQIFAN_S = 0;
				Clear_DYQ_ERR();
			}
		}
		else
		{
			K_memset(6, DYQ_Counter,sizeof(DYQ_Counter));
			if(DYQ_Counter[6] > (DYQ_BaseTimes))
			{
				DYQ_Counter[6] = 0;
				DYQ_OUT_State = Out_None;
				State.H_DYQIFAN_S = 0;
				Clear_DYQ_ERR();
			}
		}
	}
}


void Check_12V_Sta(void)
{
	if((GET_12V1_INPUT)^(GET_12V2_INPUT))	//只接了一个
		V12_I_Sta(1);
	else if((GET_12V1_INPUT)&&(GET_12V2_INPUT))
		V12_I_Sta(2);
	else
	{
		Restart_Num[Num_V12] = 0;
		V12_OUT_State = Out_None;
		BuzzerBit.Data_OErr.BitOErr.V12_Err = 0;
		DisplayBit.Data_OErr.BitOErr.V12_Err = 0;
	}
}


void Check_USB_Sta(void)
{
	if(State.USB_S)
	{
		if(GET_USB_ERROR ==0)	// USB故障
		{
			K_memset(0, USB_Counter,sizeof(USB_Counter));
			if(USB_Counter[0] > (USB_BaseTimes))
			{
				USB_Counter[0] = 0;
				UBS_Op(0);
				if(Open_Time > 8000)
				{
					Restart_Num[Num_USB]++;
					if(!DisplayBit.Data_OErr.BitOErr.USB_Err)
					{
						BuzzerBit.Data_OErr.BitOErr.USB_Err = 1;
						DisplayBit.Data_OErr.BitOErr.USB_Err = 1;
					}
				}
				BuzzerBit.Data_OErr.BitOErr.USB_Err2 = 0;
				DisplayBit.Data_OErr.BitOErr.USB_Err2 = 0;
			}
		}
		else if(GET_USB_ERROR2 ==0)
		{
			K_memset(1, USB_Counter,sizeof(USB_Counter));
			if(USB_Counter[1] > (USB_BaseTimes))
			{
				USB_Counter[1] = 0;
				if(GET_USB_INPUT == 1)		// USB有输出
					USB_OUT_State = Out_Normal;
				else
					USB_OUT_State = Out_None;
				if(!DisplayBit.Data_OErr.BitOErr.USB_Err2)
				{
					BuzzerBit.Data_OErr.BitOErr.USB_Err2 = 1;
					DisplayBit.Data_OErr.BitOErr.USB_Err2 = 1;
				}
			}
		}
		else
		{
			if(GET_USB_INPUT == 1)		// USB有输出
			{
				K_memset(2, USB_Counter,sizeof(USB_Counter));
				if(USB_Counter[2] > (USB_BaseTimes))
				{
					USB_Counter[2] = 0;
					USB_OUT_State = Out_Normal;
					Clear_USB_ERR();
				}
			}
			else
			{
				K_memset(3, USB_Counter,sizeof(USB_Counter));
				if(USB_Counter[3] > (USB_BaseTimes))
				{
					USB_Counter[3] = 0;
					USB_OUT_State = Out_None;
					Clear_USB_ERR();
				}
			}
		}
	}
}
 
void Check_AC_OUT_Sta(void)
{
	if((State.DCS_Finish_S) && (State.AC_OUT_S))
	{
		State.DCS_Finish_S = 0;
		if(((RX_BUF[AcsState]  > 0) && (RX_BUF[AcsState]  < 99)) 
		||((RX_BUF[DcsState]  > 0) && (RX_BUF[DcsState]  < 99)))	// AC_OUT 短路或者过载
		{
			K_memset(0, AC_OUT_Counter,sizeof(AC_OUT_Counter));
			if(AC_OUT_Counter[0] > (AC_OUT_BaseTimes))
			{
				AC_OUT_Counter[0] =0 ;
				AC_OUT_Op(0);
				SET_ACOUT_ERR();
			}
		}
		else
		{
			if(RX_BUF[Powbuf] > MaxPow)		// AC_OUT 输出超功率
			{
				K_memset(1, AC_OUT_Counter,sizeof(AC_OUT_Counter));
				if(AC_OUT_Counter[1] > (AC_OUT_BaseTimes))
				{
					AC_OUT_Counter[1] =0 ;
					AC_OUT_Op(0);
					SET_ACOUT_ERR();
				}
			}
#ifdef Power300W			
			else if(RX_BUF[Powbuf] > (MaxPow-100))		// 3min内关闭
			{
				K_memset(2, AC_OUT_Counter,sizeof(AC_OUT_Counter));
				if(AC_OUT_Counter[2] > (AC_OUT_BaseTimes))
				{
					AC_OUT_Counter[2] =0 ;
					AC_OUT_State = Out_Normal;
					State.AC_OV3min_S =1;
					Clear_ACOUT_ERR();
					if(ACOVTime[0] >= (3*Onemin))
					{
						AC_OUT_Op(0);
						SET_ACOUT_ERR();
					}
				}
			}
			else if(RX_BUF[Powbuf] > (MaxPow-200))		// 15min内关闭
			{
				K_memset(3, AC_OUT_Counter,sizeof(AC_OUT_Counter));
				if(AC_OUT_Counter[3] > (AC_OUT_BaseTimes))
				{
					AC_OUT_Counter[3] =0 ;
					AC_OUT_State = Out_Normal;
					State.AC_OV3min_S =0;
					State.AC_OV15min_S =1;
					ACOVTime[0] =0;
					Clear_ACOUT_ERR();
					if(ACOVTime[1] >= (15*Onemin))
					{
						AC_OUT_Op(0);
						SET_ACOUT_ERR();
					}
				}
			}
#endif			
			else if(RX_BUF[Powbuf] > 10)		// AC_OUT 有输出
			{
				K_memset(4, AC_OUT_Counter,sizeof(AC_OUT_Counter));
				if(AC_OUT_Counter[4] > (AC_OUT_BaseTimes))
				{
					AC_OUT_Counter[4] =0 ;
					AC_OUT_State = Out_Normal;
					State.AC_OV3min_S =0;
					State.AC_OV15min_S =0;
					ACOVTime[0] =0;
					ACOVTime[1] =0;
					Clear_ACOUT_ERR();
				}
			}
			else		// AC_OUT 没有输出
			{
				K_memset(5, AC_OUT_Counter,sizeof(AC_OUT_Counter));
				if(AC_OUT_Counter[5] > (AC_OUT_BaseTimes))
				{
					AC_OUT_Counter[5] =0 ;
					AC_OUT_State = Out_None;
					State.AC_OV3min_S =0;
					State.AC_OV15min_S =0;
					ACOVTime[0] =0;
					ACOVTime[1] =0;
					Clear_ACOUT_ERR();
				}
			}
		}
	}
}


void Check_Charge_Sta(void)
{
	if(GET_CH_SHORT == 1)		//短路
	{
		if(State.CH_Ch_S)
		{
			Restart_Num[Num_CH_Ch]++;
			Charger_Op(0,CH_PWM,DOWN_Dutycycle);
			BuzzerBit.Data_IErr.BitIErr.CH_Err = 1;
			DisplayBit.Data_IErr.BitIErr.CH_Err = 1;
		}
		if(State.SUN_Ch_S)
		{
			Restart_Num[Num_SUN_Ch]++;
			Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
			BuzzerBit.Data_IErr.BitIErr.SUN_Err = 1;
			DisplayBit.Data_IErr.BitIErr.SUN_Err = 1;
		}		
	}
	else if (State.H_Temp_S)
	{
		if(State.CH_Ch_S)
		{
			Charger_Op(0,CH_PWM,DOWN_Dutycycle);
			BuzzerBit.Data_IErr.BitIErr.CH_TErr = 1;
			DisplayBit.Data_IErr.BitIErr.CH_TErr = 1;
		}
		if(State.SUN_Ch_S)
		{
			Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
			BuzzerBit.Data_IErr.BitIErr.SUN_TErr = 1;
			DisplayBit.Data_IErr.BitIErr.SUN_TErr = 1;
		}
	}
	else if ((State.CH_Full_S) 
		|| (State.Charge_P_S)
		|| (DisplayBit.Data_Bat.BitBat.BatPro)
		|| (DisplayBit.Data_Bat.BitBat.B3Sc_Err))
	{
		if(State.CH_Ch_S)
			Charger_Op(0,CH_PWM,DOWN_Dutycycle);
		if(State.SUN_Ch_S)
			Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
	}
	else
	{
		if(State.CH_Ch_S)
		{
			Restart_Num[Num_CH_Ch]=0;
		 	if(AD_Data[AD_I_Charge] >= CH_I_Pro)		//充电过流
			{
				Charge_State = CH_Ch;
				if(Charge_Dutycycle > START_Dutycycle)
					Charge_Dutycycle = START_Dutycycle;
				else if(Charge_Dutycycle > 0)
					Charge_Dutycycle--;
				Charger_Op(1,CH_PWM,Charge_Dutycycle);
			}
			else if(AD_Data[AD_I_Charge] >= CH_I_Nor)	//充电电流正常
			{
				Charge_State = CH_Ch;
				Clear_CH_ERR();
				if(Charge_Dutycycle < FULL_Dutycycle)		//占空比一直很低
					State.CH_LDuty_S = 1;
				else
				{
					State.CH_LDuty_S = 0;
					CH_LowDuty_Time = 0;
				}
				if(CH_LowDuty_Time >= CHLDUTYTIME)
				{
					State.CH_LDuty_S = 0;
					CH_LowDuty_Time = 0;
					Charger_Op(0,CH_PWM,DOWN_Dutycycle);
					BuzzerBit.Data_IErr.BitIErr.CH_OIErr = 1;
					DisplayBit.Data_IErr.BitIErr.CH_OIErr = 1;
				}
			}
			else if(AD_Data[AD_I_Charge] >= CH_I_Little)	//充电电流小，缓慢增加占空比
			{
				Charge_State = CH_Ch;
				Clear_CH_ERR();
				State.CH_LDuty_S = 0;
				CH_LowDuty_Time = 0;
				if(Charge_Dutycycle < FULL_Dutycycle)
				{
					Duty_Counter[0]++;
					if(Duty_Counter[0] > 1)
					{
						Duty_Counter[0] = 0;
						Charge_Dutycycle++;
						Charger_Op(1,CH_PWM,Charge_Dutycycle);
					}
				}
			}
			else if(AD_Data[AD_I_Charge] >= CH_I_Less)	//充电电流小，增加占空比
			{
				Charge_State = CH_Ch;
				Clear_CH_ERR();
				State.CH_LDuty_S = 0;
				CH_LowDuty_Time = 0;
				if(Charge_Dutycycle < FULL_Dutycycle)
				{
					Charge_Dutycycle++;
					Charger_Op(1,CH_PWM,Charge_Dutycycle);
				}
			}
			else if(AD_Data[AD_I_Charge] >= Zero_I)	//充电电流小，迅速增加占空比
			{
				if(AD_Data[AD_I_Charge] >= CH_I_Least)
					Charge_State = CH_Ch;
				Clear_CH_ERR();
				State.CH_LDuty_S = 0;
				CH_LowDuty_Time = 0;
				if(Charge_Dutycycle < Middle_Dutycycle)
				{
					Charge_Dutycycle+=10;
					Charger_Op(1,CH_PWM,Charge_Dutycycle);
				}
				else if(Charge_Dutycycle < FULL_Dutycycle)
				{
					Charge_Dutycycle++;
					Charger_Op(1,CH_PWM,Charge_Dutycycle);
				}
//				else
//				{
//					if(AD_Data[AD_V_Bat] >= Voltage_99)
//					{
//						State.CH_Full_S = 1;
//						Charger_Op(0,CH_PWM,DOWN_Dutycycle);
//					}
//				}
			}
			else 										//没有电流
			{
				Clear_CH_ERR();
				State.CH_LDuty_S = 0;
				CH_LowDuty_Time = 0;
				if(Charge_Dutycycle < Middle_Dutycycle)
				{
					Charge_Dutycycle+=10;
					Charger_Op(1,CH_PWM,Charge_Dutycycle);
				}
				else
					Charger_Op(0,CH_PWM,DOWN_Dutycycle);
			}
		}
		else if(State.SUN_Ch_S)
		{
			Restart_Num[Num_SUN_Ch]=0;
		 	if(AD_Data[AD_I_Charge] >= SUN_I_Pro)			//充电过流
			{
				Charge_State = SUN_Ch;
				if(Charge_Dutycycle > START_Dutycycle)
					Charge_Dutycycle = START_Dutycycle;
				else if(Charge_Dutycycle > 0)
					Charge_Dutycycle--;
				Charger_Op(1,SUN_PWM,Charge_Dutycycle);
			}
			else if(AD_Data[AD_I_Charge] >= SUN_I_Nor)		//充电电流正常
			{
				Charge_State = SUN_Ch;
				Clear_SUN_ERR();
				if(Charge_Dutycycle < PR_Dutycycle)
				{	
					Duty_Counter[1]++;
					if(Duty_Counter[1] > 1)
					{
						Duty_Counter[1] = 0;
						duty = Charge_Dutycycle;
						Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
						BuzzerBit.Data_IErr.BitIErr.SUN_OIErr = 1;
						DisplayBit.Data_IErr.BitIErr.SUN_OIErr = 1;
					}
				}	
				else if(Charge_Dutycycle < (PR_Dutycycle + 10))
					State.SUN_LDuty_S = 1;
				else
				{
					State.SUN_LDuty_S = 0;
					SUN_LowDuty_Time = 0;
				}
				if(SUN_LowDuty_Time >= SUNLDUTYTIME)
				{
					State.SUN_LDuty_S = 0;
					SUN_LowDuty_Time = 0;
					duty = Charge_Dutycycle;
					Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
					BuzzerBit.Data_IErr.BitIErr.SUN_OIErr = 1;
					DisplayBit.Data_IErr.BitIErr.SUN_OIErr = 1;
				}	
			}
			else if(AD_Data[AD_I_Charge] >= SUN_I_Little)		//充电电流小，缓慢增加占空比
			{
				Charge_State = SUN_Ch;
				Clear_SUN_ERR();
				State.SUN_LDuty_S = 0;
				SUN_LowDuty_Time = 0;
				if(Charge_Dutycycle < FULL_Dutycycle)
				{
					Duty_Counter[2]++;
					if(Duty_Counter[2] > 1)
					{
						Duty_Counter[2] = 0;
						Charge_Dutycycle++;
						Charger_Op(1,SUN_PWM,Charge_Dutycycle);
					}
				}
			}
			else if(AD_Data[AD_I_Charge] >= SUN_I_Less)		//充电电流小，增加占空比
			{
				Charge_State = SUN_Ch;
				Clear_SUN_ERR();
				State.SUN_LDuty_S = 0;
				SUN_LowDuty_Time = 0;
				if(Charge_Dutycycle < FULL_Dutycycle)
				{
					Charge_Dutycycle++;
					Charger_Op(1,SUN_PWM,Charge_Dutycycle);
				}
			}
			else if(AD_Data[AD_I_Charge] >= Zero_I)		//充电电流小，迅速增加占空比
			{
				if(AD_Data[AD_I_Charge] >= SUN_I_Least)
					Charge_State = SUN_Ch;
				Clear_SUN_ERR();
				State.SUN_LDuty_S = 0;
				SUN_LowDuty_Time = 0;
				if(Charge_Dutycycle < Middle_Dutycycle)
				{
					Charge_Dutycycle+=10;
					Charger_Op(1,SUN_PWM,Charge_Dutycycle);
				}
				else if(Charge_Dutycycle < FULL_Dutycycle)
				{
					Charge_Dutycycle++;
					Charger_Op(1,SUN_PWM,Charge_Dutycycle);
				}
//				else
//				{
//					if(AD_Data[AD_V_Bat] >= Voltage_99)
//					{
//						State.CH_Full_S = 1;
//						Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
//					}
//				}
			}
			else 										//没有电流
			{
				Clear_SUN_ERR();
				State.SUN_LDuty_S = 0;
				SUN_LowDuty_Time = 0;
				if(Charge_Dutycycle < Middle_Dutycycle)
				{
					Charge_Dutycycle+=10;
					Charger_Op(1,SUN_PWM,Charge_Dutycycle);
				}
				else
					Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
			}
		}
		else	//没有在充电
		{
			Charge_State = CH_None;
		}
	}
}

void Check_Output_Sta(void)
{
	if(State.SW_DET_S)		//开关闭合
	{
		Check_DYQ_Sta();
		Check_12V_Sta();
		Check_USB_Sta();
		if((USB_OUT_State) 
		||(V12_OUT_State) 
		||(DYQ_OUT_State) 
		||(AC_OUT_State))
			State.DCH_S = 1;		
		else
			State.DCH_S = 0;	
	}
	else
		State.DCH_S = 0;	
}

void Check_Input_Sta(void)
{
	if(Charge_State)
	{
		K_memset(0, CheckInputCont,sizeof(CheckInputCont));
		if(CheckInputCont[0] > In_BaseTimes)
		{
			CheckInputCont[0] = 0;
			State.CH_S = 1;
			State.DCH_P_S = 0;
			State.AC_P_S = 0;
			LowP_Time = 0;
			BuzzerBit.Data_LowP.Byte_LowP = 0;
			DisplayBit.Data_LowP.Byte_LowP = 0;
			if(Charge_State == CH_Ch)
				State.Key_S = 0;
			if(!State.SW_DET_CH_S)
			{
//				if(State.SW_DET_S)
//					State.SW_DET_CH_S=1;
//				else
					POW_CH_Op(1);
			}
		}
	}
	else if(Access_SUN || Access_CH)
	{
		K_memset(1, CheckInputCont,sizeof(CheckInputCont));
		if(CheckInputCont[1] > In_BaseTimes)
		{
			CheckInputCont[1] = 0;
			State.CH_S = 0;
			if(Access_CH)
				State.Key_S = 0;
			if(!State.SW_DET_CH_S)
			{
//				if(State.SW_DET_S)
//					State.SW_DET_CH_S=1;
//				else
					POW_CH_Op(1);
			}
		}
	}
	else
	{
		K_memset(2, CheckInputCont,sizeof(CheckInputCont));
		if(CheckInputCont[2] > 3)
		{
			CheckInputCont[2] = 0;
			State.CH_S = 0;	
			if(State.SW_DET_CH_S)
			{
				if(State.SW_DET_S)
					State.SW_DET_CH_S=0;
				else
					POW_CH_Op(0);
			}
			if((Capacity == Capacity_0) && (!DisplayBit.Data_LowP.BitLowP.BAT_LowP))
			{
				BuzzerBit.Data_LowP.BitLowP.BAT_LowP = 1;
				DisplayBit.Data_LowP.BitLowP.BAT_LowP = 1;
			}
		}
	}
}

void Check_TempFAN(void)
{
	GetTemperature();
	if(AD_Data[AD_NTC] >= AC_P_TEMP)
		State.H_Temp_S = 1;
	else if(AD_Data[AD_NTC] < AC_PRe_TEMP)
	{
		State.H_Temp_S = 0;
		BuzzerBit.Data_IErr.Byte_IErr &= 0X77;
		DisplayBit.Data_IErr.Byte_IErr &= 0X77;
	}
	if(AD_Data[AD_NTC] >= FAN_ON_TEMP)
		State.H_TempFAN_S = 1;
	else if(AD_Data[AD_NTC] < FAN_OFF_TEMP)
		State.H_TempFAN_S = 0;
	if((State.H_TempFAN_S) || (State.H_DYQIFAN_S))
		FAN_ENABLE;
	else
		FAN_DISABLE;
}

void Operate_DYQ(void)
{
	if((!State.DYQ_S) && (AD_Data[AD_V_DYQ] > DYQ_utmost))
	{
		SET_DYQ_ERR();
	}
	else
	{
		if(Capacity < Capacity_5)	//电量不足
		{
			if(DYQ_OUT_State == Out_Normal)//正在放电
			{
				State.DCH_P_S =1 ;
				BuzzerBit.Data_LowP.BitLowP.DYQ_LowP = 1;
				DisplayBit.Data_LowP.BitLowP.DYQ_LowP = 1;
			}
		}
		else if(Capacity >= Capacity_5)
		{
			if(Restart_Num[Num_DYQ] == 0) 
			{
				if((!State.DYQ_S) && (!State.DCH_P_S) && (!DisplayBit.Data_LowP.BitLowP.DYQ_LowP))
					DYQ_Op(1);
			}
				
			else if(Restart_Num[Num_DYQ] < NumOfRestarts)
			{
				if((!State.DYQ_S) && (!State.DCH_P_S) && (!DisplayBit.Data_LowP.BitLowP.DYQ_LowP))
					State.DYQ_Restart_S = 1;
			}
			else if(Restart_Num[Num_DYQ] < (NumOfRestarts+1))
			{
				DYQ_Op(0);
				Restart_Num[Num_DYQ]++;
			}
			if(ResTime[DYQ_Time] >= RestartTime)
			{
				State.DYQ_Restart_S = 0;
				ResTime[DYQ_Time] = 0;
				DYQ_Op(1);
			}
		}
	}
}

void Operate_12V(void)
{
	if(Capacity < Capacity_5)	//电量不足
	{
		if(V12_OUT_State == Out_Normal)//正在放电
		{
			State.DCH_P_S =1 ;
			BuzzerBit.Data_LowP.BitLowP.V12_LowP = 1;
			DisplayBit.Data_LowP.BitLowP.V12_LowP = 1;
		}
	}
	else if(Capacity >= Capacity_5)
	{
		if(Restart_Num[Num_V12] == 0)
		{
			if((!State.V12_S) && (!State.DCH_P_S) && (!DisplayBit.Data_LowP.BitLowP.V12_LowP))
			{
				I_12V_PullL();
				I_12V_SetAn();
				V12_Op(1);
			}
		}
		else if(Restart_Num[Num_V12] < NumOfRestarts)
		{
			if((!State.V12_S) && (!State.DCH_P_S) && (!DisplayBit.Data_LowP.BitLowP.V12_LowP))
				State.V12_Restart_S = 1;
		}
		else if(Restart_Num[Num_V12] < (NumOfRestarts+1))
		{
			V12_Op(0);
			Restart_Num[Num_V12]++;
		}
		if(ResTime[V12_Time] >= RestartTime)
		{
			State.V12_Restart_S = 0;
			ResTime[V12_Time] = 0;
			I_12V_PullL();
			I_12V_SetAn();
			V12_Op(1);
		}
	}
}

void Operate_USB(void)
{
	if(Capacity < Capacity_5)	//电量不足
	{
		if(USB_OUT_State == Out_Normal)//正在放电
		{
			State.DCH_P_S =1 ;
			BuzzerBit.Data_LowP.BitLowP.USB_LowP = 1;
			DisplayBit.Data_LowP.BitLowP.USB_LowP = 1;
		}
	}
	else if(Capacity >= Capacity_5)
	{
		if(Restart_Num[Num_USB] == 0) 
		{
			if((!State.USB_S) && (!State.DCH_P_S) && (!DisplayBit.Data_LowP.BitLowP.USB_LowP))
				UBS_Op(1);
		}
		else if(Restart_Num[Num_USB] < NumOfRestarts)
		{
			if((!State.USB_S) && (!State.DCH_P_S) && (!DisplayBit.Data_LowP.BitLowP.USB_LowP))
				State.USB_Restart_S = 1;
		}
		else if(Restart_Num[Num_USB] < (NumOfRestarts+1))
		{
			UBS_Op(0);
			Restart_Num[Num_USB]++;
		}
		if(ResTime[USB_Time] >= RestartTime)
		{
			State.USB_Restart_S = 0;
			ResTime[USB_Time] = 0;
			UBS_Op(1);
		}
	}
}

void Operate_AC_OUT(void)
{
	if(!State.Key_S)				
	{
		AC_OUT_State = Out_None;
		if(State.AC_OUT_S) 		//AC  正在放电
			AC_OUT_Op(0);
	}
	else
	{
		if(Capacity <= Capacity_20)		//电量不足
		{
			if (!DisplayBit.Data_LowP.BitLowP.ACO_LowP)
			{
				BuzzerBit.Data_LowP.BitLowP.ACO_LowP = 1;//开启报警,报警后关闭
				DisplayBit.Data_LowP.BitLowP.ACO_LowP = 1;//开启报警,报警后关闭
			}
			if(State.AC_OUT_S) 
				State.AC_P_S = 1;	//标志ac放电到电量低保护
		}
		else
		{
			if(Restart_Num[Num_AC_OUT] == 0)
			{
				if(!State.AC_P_S)
				{
					if((!State.AC_OUT_S) 
					&& (!DisplayBit.Data_LowP.BitLowP.ACO_LowP)
					&& (!DisplayBit.Data_Bat.BitBat.ACS_Err))
						AC_OUT_Op(1);
				}
				else
					AC_OUT_Op(0);
			}
		}
	}
}

void Operate_SUN_Ch(void)
{
	u8 SUN_ErrTimes;
	if(!State.SUN_Ch_S)	
	{
		/*太阳能接入电压正常*/
		if((AD_Data[AD_V_SUN] >= C_V_SUN) 
		&& (AD_Data[AD_V_SUN] < P_V_SUN))
		{
			K_memset(1, SUN_Counter,sizeof(SUN_Counter));
			if(SUN_Counter[1] > (CH_NorTimes))
			{
				SUN_Counter[1] =0 ;
				State.SUN_NV_S = 1;
				Access_SUN = SUN_Ch;
				BuzzerBit.Data_IErr.BitIErr.SUN_VErr = 0;
				DisplayBit.Data_IErr.BitIErr.SUN_VErr = 0;
				if  ((!State.CH_Full_S) 
				&& (!State.Charge_P_S)
				&& (!State.H_Temp_S)
				&& (!DisplayBit.Data_Bat.BitBat.BatPro)
				&& (!DisplayBit.Data_Bat.BitBat.B3Sc_Err)
				&& (!DisplayBit.Data_IErr.BitIErr.SUN_OIErr))
				{
					if(Restart_Num[Num_SUN_Ch] == 0)
					{
						CH_SHORT_OUT_L();
						CH_SHORT_IN();
						Charger_Op(1,SUN_PWM,START_Dutycycle);
					}
					else if(Restart_Num[Num_SUN_Ch] < NumOfRestarts)
					{
						State.Ch_Restart_S = 1;
					}
					else if(Restart_Num[Num_SUN_Ch] < (NumOfRestarts+1))
					{
						Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
						Restart_Num[Num_SUN_Ch]++;
					}
					if(ResTime[SUN_Time] >= RestartTime)
					{
						State.Ch_Restart_S = 0;
						ResTime[SUN_Time] = 0;
						CH_SHORT_OUT_L();
						CH_SHORT_IN();
						Charger_Op(1,SUN_PWM,START_Dutycycle);
					}
				}
			}
		}
		/*太阳能接入电压过高*/
		else if(AD_Data[AD_V_SUN] >= P_V_SUN)		
		{
			K_memset(0, SUN_Counter,sizeof(SUN_Counter));
			if(SUN_Counter[0] > (CH_NorTimes))
			{
				SUN_Counter[0] =0 ;
				State.SUN_NV_S = 1;
				Access_SUN = SUN_Ch;
				BuzzerBit.Data_IErr.BitIErr.SUN_VErr = 1;
				DisplayBit.Data_IErr.BitIErr.SUN_VErr = 1;
			}
		}
		/*太阳能接入电压过低*/
		else if((AD_Data[AD_V_SUN] > N_V_SUN) && (AD_Data[AD_V_SUN] < C_V_SUN))
		{
			K_memset(2, SUN_Counter,sizeof(SUN_Counter));
			if(SUN_Counter[2] > (CH_NorTimes))
				Access_SUN = SUN_Ch;
			if(State.SUN_NV_S)
				SUN_ErrTimes = ErrTimes;
			else 
				SUN_ErrTimes = CH_NorTimes;
			if(SUN_Counter[2] > (SUN_ErrTimes))
			{
				SUN_Counter[2] = 0;
				State.SUN_NV_S = 0;
				BuzzerBit.Data_IErr.BitIErr.SUN_VErr = 1;
				DisplayBit.Data_IErr.BitIErr.SUN_VErr = 1;
			}
		}
		/*无太阳能充电器*/
		else
		{
			K_memset(3, SUN_Counter,sizeof(SUN_Counter));
			if(SUN_Counter[3] > (1))
			{
				SUN_Counter[3] =0 ;
				BuzzerBit.Data_IErr.Byte_IErr &= 0x8f;//清零4,5,6位
				DisplayBit.Data_IErr.Byte_IErr &= 0x8f;
				State.SUN_LDuty_S = 0;
				State.SUN_NV_S = 0;
				SUN_LowDuty_Time = 0;
				Restart_Num[Num_SUN_Ch] = 0;
				Access_SUN = CH_None;
			}
		}
	}
}


void Operate_CH_Ch(void)
{
	u8 CH_ErrTimes;
	if(!State.CH_Ch_S)	
	{
		/*充电器接入电压正常*/
		if((AD_Data[AD_V_Charger] >= C_V_Charger) 
		&& (AD_Data[AD_V_Charger] < P_V_Charger))
		{
			K_memset(1, Charger_Counter,sizeof(Charger_Counter));
			if(Charger_Counter[1] > (CH_NorTimes))
			{
				Charger_Counter[1] =0 ;
				Access_CH = CH_Ch;
				State.CH_NV_S = 1;
				BuzzerBit.Data_IErr.BitIErr.CH_Verr = 0;
				DisplayBit.Data_IErr.BitIErr.CH_Verr = 0;
				if(State.SUN_Ch_S)	//切换到充电器充电，关太阳能
					Charger_Op(0,SUN_PWM,DOWN_Dutycycle);
				if  ((!State.CH_Full_S) 
				&& (!State.Charge_P_S)
				&& (!State.H_Temp_S)
				&& (!DisplayBit.Data_Bat.BitBat.BatPro)
				&& (!DisplayBit.Data_Bat.BitBat.B3Sc_Err)
				&& (!DisplayBit.Data_IErr.BitIErr.CH_OIErr))
				{
					if(Restart_Num[Num_CH_Ch] == 0)
					{
						CH_SHORT_OUT_L();
						CH_SHORT_IN();
						Charger_Op(1,CH_PWM,START_Dutycycle);
					}
					else if(Restart_Num[Num_CH_Ch] < NumOfRestarts)
					{
						State.Ch_Restart_S = 1;
					}
					else if(Restart_Num[Num_CH_Ch] < (NumOfRestarts+1))
					{
						Charger_Op(0,CH_PWM,DOWN_Dutycycle);
						Restart_Num[Num_CH_Ch]++;
					}
					if(ResTime[Ch_Time] >= RestartTime)
					{
						State.Ch_Restart_S = 0;
						ResTime[Ch_Time] = 0;
						CH_SHORT_OUT_L();
						CH_SHORT_IN();
						Charger_Op(1,CH_PWM,START_Dutycycle);
					}
				}
			}
		}
		/*充电器电压太高	*/
		else if(AD_Data[AD_V_Charger] >= P_V_Charger)	
		{
			K_memset(0, Charger_Counter,sizeof(Charger_Counter));
			if(Charger_Counter[0] > (CH_NorTimes))
			{
				Charger_Counter[0] =0 ;
				Access_CH = CH_Ch;
				State.CH_NV_S = 1;
				BuzzerBit.Data_IErr.BitIErr.CH_Verr = 1;
				DisplayBit.Data_IErr.BitIErr.CH_Verr = 1;
				Operate_SUN_Ch();
			}
		}
		/*充电器电压过低*/
		else if((AD_Data[AD_V_Charger] > N_V_Charger) && (AD_Data[AD_V_Charger] < C_V_Charger))
		{
			K_memset(2, Charger_Counter,sizeof(Charger_Counter));
			if(Charger_Counter[2] > CH_NorTimes)
				Access_CH = CH_Ch;
			if(State.CH_NV_S)
				CH_ErrTimes = 2*ErrTimes;
			else 
				CH_ErrTimes = CH_NorTimes;
			if(Charger_Counter[2] > CH_ErrTimes)	//电压从正常下降下来时低电压告警延时30s
			{
				Charger_Counter[2] =0 ;
				State.CH_NV_S = 0;
				BuzzerBit.Data_IErr.BitIErr.CH_Verr = 1;
				DisplayBit.Data_IErr.BitIErr.CH_Verr = 1;
				Operate_SUN_Ch();
			}
		}
		/*小于9v，认为无充电器接入*/
		else
		{
			K_memset(3, Charger_Counter,sizeof(Charger_Counter));
			if(Charger_Counter[3] > (1))
			{
				Charger_Counter[3] =0 ;
				BuzzerBit.Data_IErr.Byte_IErr &= 0xf8;	//清零低3位
				DisplayBit.Data_IErr.Byte_IErr &= 0xf8;
				State.CH_LDuty_S = 0;
				State.CH_NV_S = 0;
				CH_LowDuty_Time = 0;
				Restart_Num[Num_CH_Ch] = 0;
				Access_CH = CH_None;
				Operate_SUN_Ch();
			}
		}
	}
}
void Operate_Out(void)
{
	if((State.SW_DET_S) 						//开关闭合
	&& (!DisplayBit.Data_Bat.BitBat.BatPro)		//电池状态正常
	&& (!DisplayBit.Data_Bat.BitBat.B3Sc_Err))	//通信正常
	{
		Operate_DYQ();
		Operate_12V();
		Operate_USB();
		Operate_AC_OUT();
	}
	else
	{
		BuzzerBit.Data_LowP.Byte_LowP = 0;
		DisplayBit.Data_LowP.Byte_LowP = 0;
		BuzzerBit.Data_OErr.Byte_OErr = 0;
		DisplayBit.Data_OErr.Byte_OErr = 0;
		memset(Restart_Num,0,4);
		memset(ResTime,0,4);	
		State.DYQ_Restart_S = 0;
		State.V12_Restart_S = 0;
		State.USB_Restart_S = 0;
		State.Key_S = 0;
		if(State.DYQ_S)		DYQ_Op(0);
		if(State.V12_S)		V12_Op(0);
		if(State.USB_S)		UBS_Op(0);
		if(State.AC_OUT_S)	AC_OUT_Op(0);
	}
}
void Operate_Light(void)
{
	if(!BuzzerBit.Data_LowP.BitLowP.ACO_LowP)		
	{
		if(State.Key_S)  		//按键按下
			LIGHT_ENABLE;
		else
			LIGHT_DISABLE;
	}
}


void Operate_Buzzer(void)   				
{
	if((BuzzerBit.Data_OErr.Byte_OErr)		// 3			
	||(BuzzerBit.Data_IErr.Byte_IErr)
	||(BuzzerBit.Data_Bat.Byte_Bat))
	{
		if(BuzzerCounter < Num_BZ)
		{
			BuzzerTime++;
			if(BuzzerTime < 2)					
				BUZZER_ENABLE;
			else if(BuzzerTime < 5)				
				BUZZER_DISABLE;
			else if(BuzzerTime < 6)				
				BUZZER_ENABLE;
			else if(BuzzerTime < 9)				
				BUZZER_DISABLE;	
			else if(BuzzerTime < 10)				
				BUZZER_ENABLE;
			else if(BuzzerTime < 21)				
				BUZZER_DISABLE;
			else if(BuzzerTime >= 21)
			{
				BuzzerTime = 0;
				BuzzerCounter++;
			}
			for(u8 i=0;i<3;i++);
		}
		else
		{
			BuzzerTime = 0;
			BuzzerCounter = 0;
			BuzzerBit.Data_IErr.Byte_IErr &= 0X77;	//清过温告警位
			BuzzerBit.Data_OErr.Byte_OErr = 0;
			BuzzerBit.Data_Bat.Byte_Bat = 0;
		}
	}
	else if(BuzzerBit.Data_LowP.Byte_LowP)
	{
		if(BuzzerCounter < Num_BZ)
		{
			BuzzerTime++;
			if(BuzzerTime < 2)					//响
				BUZZER_ENABLE;
			else if(BuzzerTime < 5)				//顿
				BUZZER_DISABLE;
			else if(BuzzerTime >= 21)
			{
				BuzzerTime = 0;
				BuzzerCounter++;
			}
			for(u8 j=0;j<3;j++);
		}
		else
		{
			if(BuzzerBit.Data_LowP.Byte_LowP & 0x0f)
			{
				UBS_Op(0);
				DYQ_Op(0);
				V12_Op(0);
				AC_OUT_Op(0);
			}
			else
				AC_OUT_Op(0);
			State.Key_S = 0;
			BuzzerTime = 0;
			BuzzerCounter = 0;
			BuzzerBit.Data_LowP.Byte_LowP = 0;
			DisplayBit.Data_LowP.BitLowP.ACO_LowP = 0;
		}
	}
	else
	{
		BUZZER_DISABLE;
		BuzzerTime = 0;
		BuzzerCounter = 0;
	}
}

	
void Check_Sta(void)
{
	Check_Capacity_Sta();
	Check_Charge_Sta();
	Check_AC_OUT_Sta();
	Check_Output_Sta();
	Check_Input_Sta();
}
void Operate_De(void)
{
	Operate_Out();
	Operate_CH_Ch();
	Operate_Light();
	Operate_Buzzer();
	Feed_Dog();
}



#if 0
void Check_AC_OUT_Sta(void)
{
	if(State.DCS_Finish_S)
	{
		State.DCS_Finish_S = 0;
		if(((RX_BUF[AcsState]  > 0) && (RX_BUF[AcsState]  < 99)) 
		||((RX_BUF[DcsState]  > 0) && (RX_BUF[DcsState]  < 99)))	// AC_OUT 短路或者过载
		{
			K_memset(0, AC_OUT_Counter,sizeof(AC_OUT_Counter));
			if(AC_OUT_Counter[0] > (AC_OUT_BaseTimes))
			{
				AC_OUT_Counter[0] =0 ;
				AC_OUT_Op(0);
				Restart_Num[Num_AC_OUT]++;
				if(!DisplayBit.Data_OErr.BitOErr.ACO_Err)
				{
					BuzzerBit.Data_OErr.BitOErr.ACO_Err = 1;
					DisplayBit.Data_OErr.BitOErr.ACO_Err = 1;
				}
			}
		}
		else
		{
			if(RX_BUF[Powbuf] > 10)		// AC_OUT 有输出
			{
				K_memset(2, AC_OUT_Counter,sizeof(AC_OUT_Counter));
				if(AC_OUT_Counter[2] > (AC_OUT_BaseTimes))
				{
					AC_OUT_Counter[2] =0 ;
					AC_OUT_State = Out_Normal;
					if(Uptime[AC_Time] >= OpDuration)
					{
						BuzzerBit.Data_OErr.BitOErr.ACO_Err = 0;
						DisplayBit.Data_OErr.BitOErr.ACO_Err = 0;
					}
				}
			}
			else		// AC_OUT 没有输出
			{
				K_memset(3, AC_OUT_Counter,sizeof(AC_OUT_Counter));
				if(AC_OUT_Counter[3] > (AC_OUT_BaseTimes))
				{
					AC_OUT_Counter[3] =0 ;
					AC_OUT_State = Out_None;
					if(Uptime[AC_Time] >= OpDuration)
					{
						BuzzerBit.Data_OErr.BitOErr.ACO_Err = 0;
						DisplayBit.Data_OErr.BitOErr.ACO_Err = 0;
					}
				}
			}
		}
	}
}
 #endif


