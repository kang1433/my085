


#include "pwm.h"


void Charge_IOInit(void)
{
	/*PWM_Charge*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_EN_Charge |PIN_EN_SUN;						 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;					//高速输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推完输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(PORT_EN_Charge, PINSOURCE_Charge, GPIO_AF_2);	//复用配置
	GPIO_PinAFConfig(PORT_EN_SUN, PINSOURCE_SUN, GPIO_AF_2);		//复用配置
}

/************************************************
函数名称 ： TIM1_CH1_PWM
功	  能 ： 定时器1通道1输出PWM
返 回 值 ： 无
*************************************************/
void TIM1_CH1_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* TIM1时基单元配置 */
	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_PRESCALER_VALUE;		 //预分频值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		 //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = TIM1_PERIOD;					 //定时周期(自动从装载寄存器ARR的值)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			 //时钟分频因子
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* TIM1通道1:PWM1模式配置 */
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				  //输出PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //使能输出
	TIM_OCInitStructure.TIM_Pulse = 0;									  //脉宽值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			  //输出极性
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	/* TIM1 主输出使能 */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

void Charger_Op(u8 sta, u8 Channel,u8 Dutycycle)	
{
	u16 tim1_pulse;
	/*计算出脉宽值(决定PWM占空比)*/
	tim1_pulse  = (TIM1_PERIOD + 1)*Dutycycle / 100;			
	if(sta)
	{
//		mPrintf(" Ch%d%\r\n",Dutycycle);
		if(Channel)
		{
			State.SUN_Ch_S = sta;
			TIM1->CCR4 = 0;
			TIM1->CCR1 = tim1_pulse;
		}
		else
		{
			State.CH_Ch_S = sta;
			TIM1->CCR1 = 0;
			TIM1->CCR4 = tim1_pulse;
		}
	}
	else
	{
		mPrintf(" Ch_L\r\n");
		Charge_Dutycycle=START_Dutycycle;
		Charge_State = CH_None;
		if(Channel)
		{
			State.SUN_Ch_S = sta;
			Uptime[SUN_Time] = 0;
		}
		else
		{
			State.CH_Ch_S = sta;
			Uptime[Ch_Time] = 0;
		}
		TIM1->CCR1 = 0;
		TIM1->CCR4 = 0;
	}
}
