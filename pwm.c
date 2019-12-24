


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
	TIM_DeInit(TIM1);
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



void TIM1_PWM_Config (void)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 ,ENABLE);
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8
//								|GPIO_Pin_9
//								|GPIO_Pin_10
								|GPIO_Pin_11;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_2);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_2); 

	TIM_DeInit(TIM1);

	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_PRESCALER_VALUE;		//预分频值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数模式
	TIM_TimeBaseStructure.TIM_Period = TIM1_PERIOD;					//定时周期(自动从装载寄存器ARR的值)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//时钟分频因子
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode        = TIM_OCMode_PWM1;			// 比较输出模式 
	TIM_OCInitStructure.TIM_OutputState   = TIM_OutputState_Enable;		// 比较输出使能 
	TIM_OCInitStructure.TIM_OutputNState  = TIM_OutputNState_Disable;	// 比较互补输出使能 
	TIM_OCInitStructure.TIM_OCPolarity    = TIM_OCPolarity_High;			// 输出极性 
	TIM_OCInitStructure.TIM_OCNPolarity   = TIM_OCNPolarity_Low;			// 互补输出极性 
	TIM_OCInitStructure.TIM_OCIdleState   = TIM_OCIdleState_Reset;		// 空闲状态下比较输出状态 
	TIM_OCInitStructure.TIM_OCNIdleState  = TIM_OCNIdleState_Reset;		// 空闲状态下比较互补输出状态 
	TIM_OCInitStructure.TIM_Pulse         = 0; 							// 脉冲宽度 

	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
//	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	TIM_ARRPreloadConfig(TIM1, ENABLE);//
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
//	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
//	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);

	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //PWM输出使能
}



void Charger_Op(u8 sta, u8 Channel,u8 Dutycycle)	
{
	u16 tim1_pulse;
	/*计算出脉宽值(决定PWM占空比)*/
	tim1_pulse  = (TIM1_PERIOD + 1)*Dutycycle / 100;			
	if(sta)
	{
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
		Charge_Dutycycle=START_Dutycycle_SUN;
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
