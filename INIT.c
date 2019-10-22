


#include "Init.h"



void PORTInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	/*EN_12V*/
	GPIO_InitStructure.GPIO_Pin = PIN_EN_12V;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(PORT_EN_12V,&GPIO_InitStructure); 	
	V12_DISABLE;				

	/*EN_DYQ*/
	GPIO_InitStructure.GPIO_Pin = PIN_EN_DYQ;	
	GPIO_Init(PORT_EN_DYQ,&GPIO_InitStructure);
	DYQ_DISABLE;

	/*EN_USB*/
	GPIO_InitStructure.GPIO_Pin = PIN_EN_USB;	
	GPIO_Init(PORT_EN_USB,&GPIO_InitStructure);
	USB_DISABLE;
	
	/*AC_Out*/
	GPIO_InitStructure.GPIO_Pin = PIN_EN_AC_OUT;	
	GPIO_Init(PORT_EN_AC_OUT,&GPIO_InitStructure);
	AC_OUT_ENABLE;	//开启逆变
	
	/*FAN */
	GPIO_InitStructure.GPIO_Pin = PIN_FAN ; 	
	GPIO_Init(PORT_FAN,&GPIO_InitStructure); 	
	FAN_DISABLE	;

	/*BUZZER */
	GPIO_InitStructure.GPIO_Pin = PIN_BZ; 	
	GPIO_Init(PORT_BZ,&GPIO_InitStructure); 
	BUZZER_DISABLE;		
	
	/*EN_POW */
	GPIO_InitStructure.GPIO_Pin = PIN_EN_POW; 	
	GPIO_Init(PORT_EN_POW,&GPIO_InitStructure); 
	POW_DISABLE;

	/*EN_LCD_LIGHT */
	GPIO_InitStructure.GPIO_Pin = PIN_LCD_LIGHT; 	
	GPIO_Init(PORT_LCD_LIGHT,&GPIO_InitStructure); 	
	LCD_L_DISABLE;
	
	/*BT_STA	 */
	GPIO_InitStructure.GPIO_Pin = PIN_BT_STA; 	
	GPIO_Init(PORT_BT_STA,&GPIO_InitStructure); 	
	BT_STA_ENABLE;	//唤醒采集板

	GPIO_InitStructure.GPIO_Pin =  PIN_LIGHT; 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 		
	GPIO_Init(PORT_LIGHT,&GPIO_InitStructure); 			
	LIGHT_DISABLE;	
	
	
	/*CH_SHORT*/
 	GPIO_InitStructure.GPIO_Pin = PIN_CH_SHORT; 	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   		
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  		
 	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
 	GPIO_Init(PORT_CH_SHORT,&GPIO_InitStructure); 
	
	/*USB_INPUT*/
	GPIO_InitStructure.GPIO_Pin = PIN_USB_INPUT; 	
	GPIO_Init(PORT_USB_INPUT,&GPIO_InitStructure); 	

	/*DYQ_PG*/
 	GPIO_InitStructure.GPIO_Pin = PIN_DYQ_PG; 	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   		
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  		
 	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(PORT_DYQ_PG,&GPIO_InitStructure); 	
	
	/*USB_ERROR*/
	GPIO_InitStructure.GPIO_Pin = PIN_USB_ERROR; 	
	GPIO_Init(PORT_USB_ERROR,&GPIO_InitStructure); 	

	/*12V1_INPUT*/
	GPIO_InitStructure.GPIO_Pin = PIN_12V1_INPUT; 	
	GPIO_Init(PORT_12V1_INPUT,&GPIO_InitStructure); 	

	/*12V2_INPUT*/
	GPIO_InitStructure.GPIO_Pin = PIN_12V2_INPUT; 	
	GPIO_Init(PORT_12V2_INPUT,&GPIO_InitStructure); 	
}

void SWD_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14; 	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   		
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  		
 	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
 	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	mPrintf(" SWD_In\r\n");
}



void CH_SHORT_OUT_L(void)
{
	if(GET_CH_SHORT == 1)
	{
		/*CH_SHORT*/
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = PIN_CH_SHORT;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
		GPIO_Init(PORT_CH_SHORT,&GPIO_InitStructure);		
		CH_SHORT_L;
		for(u16 n=0;n <10000;n++);
	}
}
void CH_SHORT_IN(void)
{
	/*CH_SHORT*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_CH_SHORT; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  		
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(PORT_CH_SHORT,&GPIO_InitStructure); 			
}

void I_12V_PullL(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_I_12V; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(PORT_I_12V,&GPIO_InitStructure);		
	I_V12_L;
	for(u16 n=0;n <10000;n++);
}
void I_12V_SetAn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  PIN_I_12V; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(PORT_I_12V,&GPIO_InitStructure); 		
}
void AC_OUT_Op(u8 sta )	
{
	State.AC_OUT_S = sta;
	if(sta)
	{
		mPrintf("AC_O_H\r\n");
	}
	else
	{
		mPrintf("AC_O_L\r\n");
		AC_OUT_State = Out_None;
		Uptime[AC_Time] = 0;
		State.Key_S = 0;
	}
}
void V12_Op(u8 sta )		
{
	State.V12_S = sta;
	if(sta)
	{
		mPrintf(" V12_H\r\n");
		V12_ENABLE;
	}
	else
	{
		mPrintf(" V12_L\r\n");
		V12_DISABLE;
		V12_OUT_State = Out_None;
		Uptime[V12_Time] = 0;
	}
}

void DYQ_Op(u8 sta )		
{
	State.DYQ_S = sta;
	if(sta)
	{
		mPrintf("DYQ_H\r\n");
		DYQ_ENABLE;
	}
	else
	{
		mPrintf("DYQ_L\r\n");
		DYQ_DISABLE;
		DYQ_OUT_State = Out_None;
		Uptime[DYQ_Time] = 0;
	}
}

void UBS_Op(u8 sta )		
{
	State.USB_S = sta;
	if(sta)
	{
		mPrintf(" USB_H\r\n");
		USB_ENABLE;
	}
	else
	{
		mPrintf(" USB_L\r\n");
		USB_DISABLE;
		USB_OUT_State = Out_None;
		Uptime[USB_Time] = 0;
	}
}

void POW_Op(u8 sta)
{
	u32 i=0;
	State.SW_DET_S = sta;
	if(sta)
	{
		mPrintf(" POW_H\r\n");
		POW_ENABLE;
		Vk1024B_Exit_Standby();
		LCD_L_ENABLE;
		BUZZER_ENABLE;
		for(i=0;i < 1500000;i++);
		BUZZER_DISABLE;
	}
	else
	{
		BUZZER_ENABLE;
		for(i=0;i < 1500000;i++);
		BUZZER_DISABLE;
		mPrintf(" POW_L\r\n");
		Vk1024B_Enter_Standby();
		LCD_L_DISABLE;
		POW_DISABLE;
	}
}
void POW_CH_Op(u8 sta)
{
	State.SW_DET_CH_S = sta;
	if(sta)
	{
		mPrintf(" POW_CH_H\r\n");
		POW_ENABLE;
		Vk1024B_Exit_Standby();
		LCD_L_ENABLE;
	}
	else
	{
		mPrintf(" POW_CH_L\r\n");
		LCD_L_DISABLE;
		Vk1024B_Enter_Standby();
		POW_DISABLE;
	}
}

void K_memset(u8 num,u8* p_counter,u8 size)
{
	u8 cache;
	if(num >= size)
	{
		printf("K_memset err");
		return ;	//错误，越界
	}
	(*(p_counter + num))++;
	cache = (*(p_counter + num));
	memset(p_counter,0,size);
	(*(p_counter + num)) = cache;
}
u16 ChartoDecimal(u8 *src)
{	
	u16 i=0;	
	u16 Dec=0;	
	for(;i<10;i++)
	{		
		if(src[i]>='0'&&src[i]<='9')
		{			
			Dec = src[i++]-'0';	
			break;		
		}	
	}	
	if(i>=10)return 0xFFFF;
	for(;i<10;i++)
	{		
		if(src[i]<'0'||src[i]>'9')
		{	
			break;		
		}		
		Dec *=10;		
		Dec += src[i]-'0';	
	}	
	return Dec;

//	int value = 0;
//	while(*src >= '0' && *src <= '0')
//	{
//		value *= 10;
//		value += *src - '0';
//		src++;
//	}
//	if(*src != '\0')
//		value = 0;
//	return value;	
}



