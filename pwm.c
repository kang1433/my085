


#include "pwm.h"


void Charge_IOInit(void)
{
	/*PWM_Charge*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_EN_Charge |PIN_EN_SUN;						 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;					//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(PORT_EN_Charge, PINSOURCE_Charge, GPIO_AF_2);	//��������
	GPIO_PinAFConfig(PORT_EN_SUN, PINSOURCE_SUN, GPIO_AF_2);		//��������
}

/************************************************
�������� �� TIM1_CH1_PWM
��	  �� �� ��ʱ��1ͨ��1���PWM
�� �� ֵ �� ��
*************************************************/
void TIM1_CH1_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* TIM1ʱ����Ԫ���� */
	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_PRESCALER_VALUE;		 //Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		 //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = TIM1_PERIOD;					 //��ʱ����(�Զ���װ�ؼĴ���ARR��ֵ)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			 //ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* TIM1ͨ��1:PWM1ģʽ���� */
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				  //���PWM1ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //ʹ�����
	TIM_OCInitStructure.TIM_Pulse = 0;									  //����ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			  //�������
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	/* TIM1 �����ʹ�� */
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

	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_PRESCALER_VALUE;		//Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = TIM1_PERIOD;					//��ʱ����(�Զ���װ�ؼĴ���ARR��ֵ)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode        = TIM_OCMode_PWM1;			// �Ƚ����ģʽ 
	TIM_OCInitStructure.TIM_OutputState   = TIM_OutputState_Enable;		// �Ƚ����ʹ�� 
	TIM_OCInitStructure.TIM_OutputNState  = TIM_OutputNState_Disable;	// �Ƚϻ������ʹ�� 
	TIM_OCInitStructure.TIM_OCPolarity    = TIM_OCPolarity_High;			// ������� 
	TIM_OCInitStructure.TIM_OCNPolarity   = TIM_OCNPolarity_Low;			// ����������� 
	TIM_OCInitStructure.TIM_OCIdleState   = TIM_OCIdleState_Reset;		// ����״̬�±Ƚ����״̬ 
	TIM_OCInitStructure.TIM_OCNIdleState  = TIM_OCNIdleState_Reset;		// ����״̬�±Ƚϻ������״̬ 
	TIM_OCInitStructure.TIM_Pulse         = 0; 							// ������ 

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
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //PWM���ʹ��
}



void Charger_Op(u8 sta, u8 Channel,u8 Dutycycle)	
{
	u16 tim1_pulse;
	/*���������ֵ(����PWMռ�ձ�)*/
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
