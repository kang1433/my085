
#include "include.h"


void TaskInit(void)
{
	while(task_add(5,150,CheckSWDET,1));
	while(task_add(6,150,ADC_Filter,1));
	while(task_add(7,2000,Check_TempFAN,1));
	while(task_add(1,150,Operate_De,1));
	while(task_add(2,150,Check_Sta,1));
	while(task_add(3,400,DisplayTask,1));
	while(task_add(4,400,Running_light,1));
	while(task_add(8,1000,Inquire,1));
	while(task_add(9,20,ACSDCSB3S_State,1));
}

#define APPLICATION_ADDRESS     ((uint32_t)0x08005000)

void IAP_Set()
{
   uint32_t i = 0;

	/* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  

	/* Copy the vector table from the Flash (mapped at the base of the application
	load address 0x08005000) to the base address of the SRAM at 0x20000000. */

	for(i = 0; i < 48; i++)//先将”应用程序“的中断向量表拷贝至SRAM中
	{
		*((uint32_t*)(0x20000000 + (i << 2)))=*(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}

	/* Enable the SYSCFG peripheral clock*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 

	/* Remap SRAM at 0x00000000 */

	/*然后在配置SYSCFG寄存器的最低两位，将SRMA映射至地址0，这样，在发生中断后，CPU从地址0取中断向量，
	这样实际上就是从SRAM中取的中断向量，而SRAM中的中断向量表又是之前从”应用程序“的中断向量表拷贝过
	来的，所以，最终其实是取的”应用程序“的中断向量表。*/
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
}	

void StartInitialization(void)
{
	Capacity=Capacity_100;
	Vk1024B_DisAll(0);
	while(GET_SW_DET==1)	//15ms
	{
		if(((Open_Time%300) < 20) && ((Open_Time%300) > 10))
		{
			Feed_Dog();
			printf("B3S?\r\n");
		}
		ACSDCSB3S_State();
		Capacity_Init();
		if(((Open_Time%300) < 215) && ((Open_Time%300) > 200))
			printf("DCOFF\r\n");			
		if(Open_Time > 2000)
		{
			my_SWD_In();
			State.SW_DET_Op_S=1;
			POW_Op(1);
			break;
		}
	}
	if(State.SW_DET_S)
	{
		while(Open_Time < 4000)
		{
			ADC_Filter();
			Vk1024B_DisAll(1);
			Capacity_Init();
			Feed_Dog();
		}
	}
	else
	{
		while(Open_Time < 2000) 
		{
			if((Open_Time%300) < 20)
			{
				printf("B3S?\r\n");
				Operate_CH_Ch();
				Check_Charge_Sta();
				Feed_Dog();
			}
			ACSDCSB3S_State();
			Capacity_Init();
			if(((Open_Time%300) < 215) && ((Open_Time%300) > 200))
				printf("DCOFF\r\n");			
			if(Access_SUN || Access_CH)
			{
				POW_CH_Op(1);
				Vk1024B_DisAll(1);
			}
			ADC_Filter();
		}
	}
	memset(SOC_Counter,0,sizeof(SOC_Counter));
	memset(Vol_Counter,0,sizeof(Vol_Counter));
}

main()
{
	SysTickInit();
	PORTInit();
	Charge_IOInit();
	Uart_Init(9600);
	TIM1_CH1_PWM_Init();
	KyeConfig();
	ADC1_Init();
	Vk1024B_Init();
	my_WDG_Config();
	TaskInit();
	StartInitialization();
	while(1)
	{
		task_run();
	}
}




