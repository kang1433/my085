 /**********************************************************************
Copyright: 2016,量道新能源科技有限公司

文件名称：ADC.c

***********************************************************************/

#include "Adc.h"

u16 AD_Value[AD_ChNUM];      					//AD采集值
int32 AD_Data[AD_ChNUM];      					//AD计算值
__IO uint16_t ADC_ConvertedValue[AD_Count][AD_ChNUM];


const float  Resistance[TempSize]=
{1746.4135,1641.9976,1544.4906,1453.3933,1368.2451,1288.6215,1214.1307,1144.4111,1079.1283,1017.9737,
960.6618,906.9284,856.5290,809.2373,764.8435,723.1532,683.9862,647.1753,612.5652,580.0118,//-20~-11
549.3812,520.5489,493.3992,467.8245,443.7247,421.0064,399.5828,379.3730,360.3014,342.2979,//-10~-1
325.2967,309.2365,294.0603,279.7145,266.1493,253.3180,241.1770,229.6855,218.8053,208.5006,//0~9
198.7381,189.4862,180.7157,172.3990,164.5102,157.0252,149.9212,143.1768,136.7721,130.6880,//10~19
124.9071,119.4126,114.1890,109.2214,104.4961,100.0000,95.7209,91.6472,87.7681,84.0733,//20~29
80.5532,77.1985,74.0008,70.9520,68.0443,65.2706,62.6241,60.0982,57.6870,55.3847,//30~39
53.1858,51.0852,49.0781,47.1598,45.3260,43.5726,41.8957,40.2915,38.7567,37.2878,//40~49
35.8818,34.5356,33.2465,32.0117,30.8288,29.6953,28.6088,27.5674,26.5688,25.6111,//50~59
24.6926,23.8113,22.9656,22.1540,21.3748,20.6267,19.9083,19.2183,18.5554,17.9184,//60~69
17.3063,16.7179,16.1522,15.6082,15.0851,14.5819,14.0977,13.6319,13.1835,12.7520,//70~79
12.3365,11.9364,11.5511,11.1799,10.8224,10.4779,10.1458,9.8258,9.5173,9.2198,//80~89
8.9330,8.6563,8.3894,8.1319,7.8835,7.6437,7.4122,7.1888,6.9730,6.7647,//90~99
6.5635};


 void ADC1_GPIO_Init(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;

	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 
//							 	|GPIO_Pin_1 
							 	|GPIO_Pin_2 
							 	|GPIO_Pin_3 
							 	|GPIO_Pin_4 
							 	|GPIO_Pin_5 
							 	|GPIO_Pin_6 ;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 
														 	|GPIO_Pin_1 ;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
 }

 
 /**配置ADC1的工作模式 */
 static void ADC1_Mode_Init(void)
 {
	 ADC_InitTypeDef	 ADC_InitStructure;
 
	 /*设置ADC分频因子为4 ,ADC最大时间不能超过14M*/
	 RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4); 
	 /* ADC1 Periph clock enable */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	 /* ADC1 DeInit */	
	 ADC_DeInit(ADC1);	 
	 /*初始化ADC结构体，此句必须加，不加的话多路ADC数据会交换*/
	 ADC_StructInit(&ADC_InitStructure);						 
 
	 /*配置ADC分辨率为12位*/
	 ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	 /*开启连续转换*/
	 ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	 /*禁止触发检测，使用软件触发*/
	 ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	 /*ADC采集数据右对齐*/
	 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	 /*向上扫描*/
	 ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	 /*ADC初始化*/
	 ADC_Init(ADC1, &ADC_InitStructure); 
							   
	 /*配置采样通道及时间*/
	 ADC_ChannelConfig(ADC1,	 ADC_Channel_0
//							|ADC_Channel_1
							|ADC_Channel_2
							|ADC_Channel_3
							|ADC_Channel_4
							|ADC_Channel_5
							|ADC_Channel_6
//							|ADC_Channel_7
							|ADC_Channel_8
							|ADC_Channel_9, ADC_SampleTime_239_5Cycles); //单通道时间21us
 
	 /* ADC 校准 */
	 ADC_GetCalibrationFactor(ADC1);
 
	 /* 循环模式下的 ADC DMA 请求 */
	 ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
 
	 /* 使能 ADC_DMA */
	 ADC_DMACmd(ADC1, ENABLE);	
 
	 /* 使能 ADC1 */
	 ADC_Cmd(ADC1, ENABLE); 	
 
	 /* 等待 ADCEN 标志 */
	 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));
		 
	 /*  ADC1 常规软件启动转换 */  
	 ADC_StartOfConversion(ADC1);	 
 }


 
 /** DMA 工作方式配置  */
 static void ADC1_DMA_Init(void)
 {
	 DMA_InitTypeDef   DMA_InitStructure;
	 NVIC_InitTypeDef  NVIC_InitStructure;
	 /* 使能DMA时钟 */
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
 
	 /* 复位DMA1_Channel1 */
	 DMA_DeInit(DMA1_Channel1);
	 
	 /*DMA外设ADC基地址*/
	 DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR;// (uint32_t)ADC1_DR_Address;//(uint32_t)&ADC1->DR;
	 
	 /*DMA内存基地址*/
	 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	 
	 /*外设作为数据传输源*/
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	 
	 /*DMA通道的DMA缓存的大小*/
	 DMA_InitStructure.DMA_BufferSize = AD_ChNUM * AD_Count;
	 
	 /*外设地址寄存器不变*/
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 
	 /*内存地址寄存器递增*/
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 
	 /*外设取值大小设置为半字*/
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	 
	 /*数据大小设置为半字*/
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	 
	 /*DMA循环模式，即完成后重新开始覆盖*/
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	 
	 /*DMA优先级设置为高*/
	 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	 
	 /*DMA通道x禁用内存到内存传输*/
	 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	 
	 /*DMA初始化*/
	 DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	 
	 /*清除一次DMA中断标志*/
	 DMA_ClearITPendingBit(DMA1_IT_TC1);								 
	 
	 /*使能DMA传输完成中断*/
	 DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE); 
	 
	 /*选择DMA1通道中断*/
	 NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  
 
	 /*中断使能*/ 
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	 
	 /*优先级设为0 */
	 NVIC_InitStructure.NVIC_IRQChannelPriority = 0;  
	 
	 /*使能 DMA 中断*/ 
	 NVIC_Init(&NVIC_InitStructure);  
  
	 /* DMA1 Channel1 enable */
	 DMA_Cmd(DMA1_Channel1, ENABLE);
 }

 /**  ADC1初始化  */
 void ADC1_Init(void)
 {
	 ADC1_GPIO_Init();
	 ADC1_Mode_Init();
	 ADC1_DMA_Init();
 }

 
 signed short CalculateTemperature(float R)
{
	int   iTem;
	u8 i;
	u8 cBottom, cTop;
	cBottom = 0; 
	cTop    = TempTop;
	for (i=TempMid; (cTop-cBottom)!=1; )        
	{
		if (R > Resistance[i])
		{
			cTop = i;
			i = (cTop + cBottom) / 2;
		}
		else if (R < Resistance[i])
		{
			cBottom = i;
			i = (cTop + cBottom) / 2;
		}
		else
		{
			iTem = (int)i - 30;
			return iTem;
		}
	}
	if((R-Resistance[cBottom])>(R-Resistance[cTop]))
	{
		iTem = (int)cTop  - 30;
	}
	else
	{
		iTem = (int)cBottom  - 30;
	}
	return iTem;
}

void GetTemperature(void)
{
	float Res;
	Res=((((float)AD_Value[AD_NTC]*100))/(4096-((float)AD_Value[AD_NTC])));
	if (Res > Resistance[0])               
		AD_Data[AD_NTC]= -999;
	else if (Res < Resistance[TempTop])       
		AD_Data[AD_NTC]= 999;
	else
		AD_Data[AD_NTC]=CalculateTemperature(Res);
}



void ADC_Filter(void)
{
	u32 sum=0;
	u8 i,ChNUM,Count;
	u16 datatemp[AD_Count];
	double temp[AD_ChNUM];
	if(State.ADC_Finish_S)
	{
		for(ChNUM=0;ChNUM<AD_ChNUM;ChNUM++)
		{
			for(Count=0;Count<AD_Count;Count++)
				datatemp[Count]= ADC_ConvertedValue[Count][ChNUM];
			FILTER_UI(datatemp,AD_Count);
			for(i=15; i<(AD_Count -15); i++)   // 去掉15个最大值 和最小值
				sum += datatemp[i]; 
			AD_Value[ChNUM] = sum /(AD_Count -30);
			sum =0;
		}
		temp[AD_I_12V]=(double)3300000/4096/31.1/5.375;
		temp[AD_I_Charge]=(double)3300000/4096/31.1/5;// 4.82;
		temp[AD_I_DYQ]=(double)3300000/4096/DYQRATIO;
		temp[AD_V_Charger]=(double)3300*10.78/4096;
		temp[AD_V_SUN]=(double)3300*10.78/4096;
		temp[AD_V_Bat]=(double)3300*11/4096;
		temp[AD_V_DYQ]=(double)3300*11/4096;
		AD_Data[AD_I_12V]=(temp[AD_I_12V] * (double)AD_Value[AD_I_12V]);						// mA
		AD_Data[AD_I_Charge]=(temp[AD_I_Charge] * (double)AD_Value[AD_I_Charge]);				// mA
		AD_Data[AD_I_DYQ]=(temp[AD_I_DYQ] * (double)AD_Value[AD_I_DYQ]);						// mA
		AD_Data[AD_V_Charger]=(temp[AD_V_Charger] * (double)AD_Value[AD_V_Charger]);			// mV
		AD_Data[AD_V_SUN]=(temp[AD_V_SUN] * (double)AD_Value[AD_V_SUN]);					// mV
		AD_Data[AD_V_Bat]=(temp[AD_V_Bat] * (double)AD_Value[AD_V_Bat]);						// mV
		AD_Data[AD_V_DYQ]=(temp[AD_V_DYQ] * (double)AD_Value[AD_V_DYQ]);					// mV
		State.ADC_Finish_S = 0;
	}
}


/*DMA1_Channel1中断服务函数*/
void DMA1_Channel1_IRQHandler()  
{  
	/*判断DMA传输完成中断*/ 
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)                        
	{ 
		State.ADC_Finish_S = 1;
	}	
	/*清除DMA中断标志位*/	
	DMA_ClearITPendingBit(DMA1_IT_TC1);                      
}  


/*************************************
*名称：FILTER_UI(unsigned int *BUFFER)
*功能: 冒泡排序
*输入：
*输出：
**************************************/
void FILTER_UI(u16 *BUFFER,u8 num)
{
	u8 i,j;
	u16 temp;
	for(i = 0; i < num; i++)
	{
		for(j = i; j < num; j++)
		{
			if(BUFFER[i] < BUFFER[j])
			{
				temp = BUFFER[i];
				BUFFER[i] = BUFFER[j];
				BUFFER[j] = temp;
			}
		}
	}
}

