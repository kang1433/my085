/**********************************************************************
***********************************************************************/
#include "KEY.h"


void KyeConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* key引脚配置 */
	GPIO_InitStructure.GPIO_Pin =  PIN_KEY; 						   //按键
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;					   //输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;				   //无上下拉(浮空)
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(PORT_KEY, &GPIO_InitStructure);

	/*SW_DET*/
	GPIO_InitStructure.GPIO_Pin = PIN_SW_DET;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(PORT_SW_DET,&GPIO_InitStructure); 		

	/* EXTI配置*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;							//外部中断线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 			//中断请求（请求模式）
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource1);	   //EXIT中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;				   //外部中断
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void CheckSWDET(void)
{
	u32 i =0 ;
	if(GET_SW_DET == 1)		//开关闭合
	{
		State.SW_DET_Press_S=1;
		if((SW_DET_PressTime>2000)&&(!State.SW_DET_Op_S))
		{
			State.SW_DET_Op_S=1;
			if(State.SW_DET_S)	//已经是开机状态
			{
				if(State.SW_DET_CH_S)//有充电器在
				{
					State.SW_DET_S = 0;	//置位关机
					BUZZER_ENABLE;
					for(i=0;i < 1500000;i++);
					BUZZER_DISABLE;
				}
				else
					POW_Op(0);
			}
			else
				POW_Op(1);
		}
	}
	else
	{
		State.SW_DET_Op_S=0;
		State.SW_DET_Press_S=0;
		SW_DET_PressTime=0;
	}
	if(LowP_Time>= LOWPCLOSETIME)	// || (Open_Time >= CLOSETIME))
	{
		LowP_Time = 0;
//		Open_Time = 30000;
		if(State.SW_DET_CH_S)//有充电器在
		{
			State.SW_DET_S = 0;	//置位关机
			BUZZER_ENABLE;
			for(i=0;i < 1500000;i++);
			BUZZER_DISABLE;
		}
		else
			POW_Op(0);
	}
}

void EXTI0_1_IRQHandler(void)
{
	if(((EXTI->PR & EXTI_Line1) != (uint32_t)RESET) 
		&& (State.SW_DET_S)
		&& (!DisplayBit.Data_Bat.BitBat.ACS_Err))
	{
		if(GET_KEY==0)	//按下
		{
			mPrintf("+");
			State.Key_Press_S = 1; 		//标志按下
		}
		if(GET_KEY== 1) //松开
		{
			if(Key_PressTime > 50)		//按下有50ms
			{
				mPrintf("-");
				if((BuzzerBit.Data_OErr.BitOErr.ACO_Err)
				||(BuzzerBit.Data_LowP.BitLowP.ACO_LowP))
				{
					BUZZER_DISABLE;
					State.Key_S = 0;
					BuzzerBit.Data_LowP.BitLowP.ACO_LowP = 0;
					DisplayBit.Data_LowP.BitLowP.ACO_LowP = 0;
					BuzzerBit.Data_OErr.BitOErr.ACO_Err = 0;
					DisplayBit.Data_OErr.BitOErr.ACO_Err = 0;
					Restart_Num[Num_AC_OUT] = 0;
				}
				else if(DisplayBit.Data_OErr.BitOErr.ACO_Err)
				{
					State.Key_S = 0;
					DisplayBit.Data_OErr.BitOErr.ACO_Err = 0;
					DisplayBit.Data_LowP.BitLowP.ACO_LowP = 0;
					Restart_Num[Num_AC_OUT] = 0;
				}
//				else if((Charge_State == CH_Ch) || (Access_CH))
//					State.Key_S = 0;
				else
					State.Key_S = !State.Key_S ;
			}
			State.Key_Press_S = 0;
			Key_PressTime = 0;
		}
		EXTI->PR = EXTI_Line1;					   //清除标志位
	}
	else
		EXTI->PR = 0x0003;						   //清除EXTI0_1所有标志位
}




