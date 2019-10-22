/**********************************************************************
Copyright: 2016,量道新能源科技有限公司
文件名称：LCD.C
摘要：    液晶屏刷新显示子程序
当前版本：
作者： 
完成日期： 
***********************************************************************/

#include "LCD.h"

static u8 Light_state = 0;
static u8 Flicker_state = 0;



void Display_FRAME_OUT(void)
{
	if(State.SW_DET_S)			
		Vk1024B_DisSegOn(LCD_FRAME_OUT);	
	else
		Vk1024B_DisSegOff(LCD_FRAME_OUT);	
}
void Display_Frame(void)
{
	if((DisplayBit.Data_LowP.Byte_LowP)
	||(DisplayBit.Data_Bat.BitBat.BatPro)
	||(DisplayBit.Data_Bat.BitBat.B3Sc_Err))			
	{
		if(Flicker_state)				
			Vk1024B_DisSegOn(LCD_FRAME_BAT);	
		else	
			Vk1024B_DisSegOff(LCD_FRAME_BAT);
	}
	/*常亮*/
	else
		Vk1024B_DisSegOn(LCD_FRAME_BAT);	
}

/*点烟器图标显示*/
void Display_DYQ(void)
{
	if((DisplayBit.Data_OErr.BitOErr.DYQ_Err)		//	闪烁	
	||(DisplayBit.Data_LowP.BitLowP.DYQ_LowP))
	{
		if(Flicker_state)	
			Vk1024B_DisSegOn(LCD_SMOKE);	
		else								
			Vk1024B_DisSegOff(LCD_SMOKE);	
	}
	else		
	{
		if(DYQ_OUT_State == Out_Normal)	
			Vk1024B_DisSegOn(LCD_SMOKE);	
		else	
			Vk1024B_DisSegOff(LCD_SMOKE);	
	}
}
/* 12V 图标显示*/
void Display_12V(void)
{
	if((DisplayBit.Data_LowP.BitLowP.V12_LowP)	//	闪烁	
	||(DisplayBit.Data_OErr.BitOErr.V12_Err))
	{
		if(Flicker_state)	
			Vk1024B_DisSegOn(LCD_DC12);	
		else								
			Vk1024B_DisSegOff(LCD_DC12);	
	}
	else		
	{
		if(V12_OUT_State == Out_Normal)
			Vk1024B_DisSegOn(LCD_DC12);	
		else	
			Vk1024B_DisSegOff(LCD_DC12);	
	}
}
/*USB 图标显示*/
void Display_USB(void)
{
	if((DisplayBit.Data_LowP.BitLowP.USB_LowP)	//	闪烁	
	||(DisplayBit.Data_OErr.BitOErr.USB_Err))
	{
		if(Flicker_state)	
			Vk1024B_DisSegOn(LCD_USB); 
		else								
			Vk1024B_DisSegOff(LCD_USB);	
	}
	else		
	{
		if(USB_OUT_State == Out_Normal)			
			Vk1024B_DisSegOn(LCD_USB); 
		else	
			Vk1024B_DisSegOff(LCD_USB);	
	}
}
/*AC 图标显示*/
void Display_AC_Out(void)
{
	if((DisplayBit.Data_LowP.BitLowP.ACO_LowP)	//	闪烁	
	||(DisplayBit.Data_OErr.BitOErr.ACO_Err)
	||(DisplayBit.Data_Bat.BitBat.ACS_Err))
	{
		if(Flicker_state)	
			Vk1024B_DisSegOn(LCD_AC_Out); 
		else								
			Vk1024B_DisSegOff(LCD_AC_Out); 
	}
	else		
	{
		if(State.AC_OUT_S)			
			Vk1024B_DisSegOn(LCD_AC_Out); 
		else	
			Vk1024B_DisSegOff(LCD_AC_Out); 
	}
}
/*太阳能充电图标显示*/
void Display_SUN(void)
{
	if((DisplayBit.Data_IErr.BitIErr.SUN_Err)	
	||(DisplayBit.Data_IErr.BitIErr.SUN_VErr)
	||(DisplayBit.Data_IErr.BitIErr.SUN_OIErr)) 
	{
		if(Flicker_state)	
			Vk1024B_DisSegOn(LCD_SUN_CH); 
		else								
			Vk1024B_DisSegOff(LCD_SUN_CH); 
	}
	else		
	{
		if(Charge_State == SUN_Ch)			
			Vk1024B_DisSegOn(LCD_SUN_CH); 
		else	
			Vk1024B_DisSegOff(LCD_SUN_CH); 
	}
	
}
/*市电充电图标显示*/
void Display_CH(void)
{
	if((DisplayBit.Data_IErr.BitIErr.CH_Err)	
	||(DisplayBit.Data_IErr.BitIErr.CH_Verr)
	||(DisplayBit.Data_IErr.BitIErr.CH_OIErr)) 
	{
		if(Flicker_state)	
			Vk1024B_DisSegOn(LCD_AC_CH); 
		else								
			Vk1024B_DisSegOff(LCD_AC_CH); 
	}
	else		
	{
		if(Charge_State == CH_Ch)			
			Vk1024B_DisSegOn(LCD_AC_CH); 
		else	
			Vk1024B_DisSegOff(LCD_AC_CH); 
	}
}


void Display_Warning(void)
{
	if((DisplayBit.Data_OErr.Byte_OErr)
	||(DisplayBit.Data_IErr.Byte_IErr)
	||(DisplayBit.Data_Bat.Byte_Bat))			
	{
		if(Flicker_state)	
			Vk1024B_DisSegOn(LCD_WARNING); 
		else								
			Vk1024B_DisSegOff(LCD_WARNING); 
	}
	else								
		Vk1024B_DisSegOff(LCD_WARNING); 
}

void Run_Pow(void)
{
	switch(Capacity)
	{
	case Capacity_0:
	case Capacity_5:
		Vk1024B_DisSegOff(LCD_BAT_20); 
		Vk1024B_DisSegOff(LCD_BAT_40); 
		Vk1024B_DisSegOff(LCD_BAT_60); 
		Vk1024B_DisSegOff(LCD_BAT_80); 
		Vk1024B_DisSegOff(LCD_BAT_100); 
		break;
	case Capacity_20:
		if(BuzzerBit.Data_LowP.BitLowP.ACO_LowP)//AC低电量报警
			Vk1024B_DisSegOff(LCD_BAT_20); 
		else
			Vk1024B_DisSegOn(LCD_BAT_20); 
		Vk1024B_DisSegOff(LCD_BAT_40); 
		Vk1024B_DisSegOff(LCD_BAT_60); 
		Vk1024B_DisSegOff(LCD_BAT_80); 
		Vk1024B_DisSegOff(LCD_BAT_100); 
		break;
	case Capacity_40:
		Vk1024B_DisSegOn(LCD_BAT_20); 
		Vk1024B_DisSegOn(LCD_BAT_40); 
		Vk1024B_DisSegOff(LCD_BAT_60); 
		Vk1024B_DisSegOff(LCD_BAT_80); 
		Vk1024B_DisSegOff(LCD_BAT_100); 
		break;
	case Capacity_60:
		Vk1024B_DisSegOn(LCD_BAT_20); 
		Vk1024B_DisSegOn(LCD_BAT_40); 
		Vk1024B_DisSegOn(LCD_BAT_60); 
		Vk1024B_DisSegOff(LCD_BAT_80); 
		Vk1024B_DisSegOff(LCD_BAT_100); 
		break;
	case Capacity_80:
		Vk1024B_DisSegOn(LCD_BAT_20); 
		Vk1024B_DisSegOn(LCD_BAT_40); 
		Vk1024B_DisSegOn(LCD_BAT_60); 
		Vk1024B_DisSegOn(LCD_BAT_80); 
		Vk1024B_DisSegOff(LCD_BAT_100); 
		break;
	case Capacity_100:
	default:
		Vk1024B_DisSegOn(LCD_BAT_20); 
		Vk1024B_DisSegOn(LCD_BAT_40); 
		Vk1024B_DisSegOn(LCD_BAT_60); 
		Vk1024B_DisSegOn(LCD_BAT_80); 
		Vk1024B_DisSegOn(LCD_BAT_100); 
		break;
	}
}
void Display_Power(void)
{
	if(DisplayBit.Data_Bat.BitBat.BatPro)
	{
		if(Flicker_state)	
		{
			Run_Pow();
		}
		else	//灭
		{
			Vk1024B_DisSegOff(LCD_BAT_20); 
			Vk1024B_DisSegOff(LCD_BAT_40); 
			Vk1024B_DisSegOff(LCD_BAT_60); 
			Vk1024B_DisSegOff(LCD_BAT_80); 
			Vk1024B_DisSegOff(LCD_BAT_100); 
		}
	}
	else
		Run_Pow();
}

void Running_light(void)
{ 
	if(State.DCH_S)								
	{	
		switch(Light_state)
		{
			case 0:
				Vk1024B_DisSegOff(LCD_D_DOWN);
				Vk1024B_DisSegOff(LCD_D_LEFT);
				Vk1024B_DisSegOff(LCD_D_RIGHT);
				Vk1024B_DisSegOff(LCD_D_UP);
				break;
			case 1:
				Vk1024B_DisSegOff(LCD_D_DOWN);
				Vk1024B_DisSegOff(LCD_D_LEFT);
				Vk1024B_DisSegOff(LCD_D_RIGHT);
				Vk1024B_DisSegOn(LCD_D_UP);
				break;	
			case 2:
				Vk1024B_DisSegOff(LCD_D_DOWN);
				Vk1024B_DisSegOff(LCD_D_LEFT);
				Vk1024B_DisSegOn(LCD_D_RIGHT);
				Vk1024B_DisSegOn(LCD_D_UP);
				break;
			case 3:
				Vk1024B_DisSegOn(LCD_D_DOWN);
				Vk1024B_DisSegOff(LCD_D_LEFT);
				Vk1024B_DisSegOn(LCD_D_RIGHT);
				Vk1024B_DisSegOn(LCD_D_UP);
				break;
			case 4:
				Vk1024B_DisSegOn(LCD_D_DOWN);
				Vk1024B_DisSegOn(LCD_D_LEFT);
				Vk1024B_DisSegOn(LCD_D_RIGHT);
				Vk1024B_DisSegOn(LCD_D_UP);
				break;
			default:
				Light_state = 0;
				break;
		}
	}
	else
	{
		Vk1024B_DisSegOn(LCD_D_DOWN);
		Vk1024B_DisSegOn(LCD_D_LEFT);
		Vk1024B_DisSegOn(LCD_D_RIGHT);
		Vk1024B_DisSegOn(LCD_D_UP);
	}
	if(State.CH_S)								//充电	
	{
		switch(Light_state)
		{
			case 0:
				Vk1024B_DisSegOff(LCD_C_LEFT);
				Vk1024B_DisSegOff(LCD_C_DOWN);
				Vk1024B_DisSegOff(LCD_C_UP);
				Vk1024B_DisSegOff(LCD_C_RIGHT);
				break;
			case 1:
				Vk1024B_DisSegOff(LCD_C_LEFT);
				Vk1024B_DisSegOff(LCD_C_DOWN);
				Vk1024B_DisSegOn(LCD_C_UP);
				Vk1024B_DisSegOff(LCD_C_RIGHT);
				break;	
			case 2:
				Vk1024B_DisSegOff(LCD_C_LEFT);
				Vk1024B_DisSegOff(LCD_C_DOWN);
				Vk1024B_DisSegOn(LCD_C_UP);
				Vk1024B_DisSegOn(LCD_C_RIGHT);
				break;
			case 3:
				Vk1024B_DisSegOff(LCD_C_LEFT);
				Vk1024B_DisSegOn(LCD_C_DOWN);
				Vk1024B_DisSegOn(LCD_C_UP);
				Vk1024B_DisSegOn(LCD_C_RIGHT);
				break;
			case 4:
				Vk1024B_DisSegOn(LCD_C_LEFT);
				Vk1024B_DisSegOn(LCD_C_DOWN);
				Vk1024B_DisSegOn(LCD_C_UP);
				Vk1024B_DisSegOn(LCD_C_RIGHT);
				break;
			default:
				Light_state = 0;
				break;
		}
	}
	else
	{
		Vk1024B_DisSegOn(LCD_C_LEFT);
		Vk1024B_DisSegOn(LCD_C_DOWN);
		Vk1024B_DisSegOn(LCD_C_UP);
		Vk1024B_DisSegOn(LCD_C_RIGHT);
	}		
	Light_state++;
	if((Light_state >=5 ) || ((!State.CH_S) && (!State.DCH_S)))
		Light_state = 0;
}


void DisplayTask(void)
{
	Vk1024B_DisSegOn(LCD_FRAME_IN);
//	Vk1024B_DisSegOn(LCD_FRAME_OUT);
	Display_FRAME_OUT();
	Display_Power();	
	Display_Frame();
	Display_DYQ();
	Display_12V();
	Display_USB();
	Display_AC_Out();
	Display_SUN();
	Display_CH();
	Display_Warning();
	Flicker_state = !Flicker_state;
}


