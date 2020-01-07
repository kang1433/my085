/**
  ******************************************************************************

  ******************************************************************************
  ******************************************************************************
  */	
/* Includes ------------------------------------------------------------------*/
#include "vk1024b.h"
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VK1024B_BASE100MS	226757 	//通过n个nop指令实现100mS延时，根据单片机修改此常数
#define VK1024B_CLK			10		//WR信号线频率 ,由Vk1024B_Delay实现， =10->100kHz =100->11.11kHz
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const unsigned char vk1024B_segtab[VK1024B_SEGNUM]={
	19,20,21,22,23,24
};
unsigned char vk1024B_addrbit=6;//驱动ic的ram地址线数A5-A0,
unsigned char vk1024B_segi,vk1024B_comi;
unsigned char vk1024B_maxcom=4;//驱动的com数可以是4com，3com，2com
unsigned char vk1024B_dispram[VK1024B_SEGNUM];//显示RAM buffer只使用低4位，bit0->com0,bit1->com1,bit2->com2,bit3->com3
u32 Display_Sta=0x80000000;
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  Inserts a delay time.
  * @param  ucDat-> 延时时间
  * @retval None
  */
void Vk1024B_Delay(unsigned int ucDat)
{//ucDat=10->100kHz ucDat=100->11.11kHz ucDat=226757->100mS
	while(ucDat--) 
	{//nop指令根据单片机做相应的修改
		__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();//__nop();__nop();__nop();
	}
}
/**
  * @brief  Vk1024B clk
  * @param  None 
  * @retval None
  */
void WriteClockVk1024B(void)
{
	VK1024B_WR_L(); 
	Vk1024B_Delay(VK1024B_CLK);
	VK1024B_WR_H();	
	Vk1024B_Delay(VK1024B_CLK);	
}
/**
  * @brief  Write Vk1024B Command
  * @param  FunctonCode->功能/命令码 
  * @retval None
  */
void WriteCommandVk1024B(unsigned char FunctonCode)
{
	unsigned char Shift = 0x80; 
	unsigned char i;

	VK1024B_CS_L();   //CS 片选开   
	Vk1024B_Delay(VK1024B_CLK/2);
	VK1024B_DATA_H();  
	WriteClockVk1024B();
	VK1024B_DATA_L();  
	WriteClockVk1024B();
	VK1024B_DATA_L();  
	WriteClockVk1024B();

	for(i = 0; i < 8; i++) 
	{
		if(Shift & FunctonCode) 	
			VK1024B_DATA_H(); 
		else 
			VK1024B_DATA_L();	  

		WriteClockVk1024B();
		Shift = Shift >> 1;
	}
	VK1024B_DATA_L(); 
	WriteClockVk1024B();	//发送一个 0 16xx中最后一位 X
	VK1024B_CS_H(); 			//CS 片选关  
	Vk1024B_Delay(VK1024B_CLK/2);
	VK1024B_DATA_H(); 
}
/**
  * @brief  Write 1 data to Vk1024B 
  * @param  Addr->写入ram的地址 
  * @param  Databuf->写入ram的数据
  * @retval None
  */
void Write1DataVk1024B(unsigned char Addr,unsigned char Dat)
{
	unsigned char i; 
	unsigned char Shift;
	Shift = 0x20; //屏蔽高3位 只用5位
	VK1024B_CS_L();  //CS 片选开
	Vk1024B_Delay(VK1024B_CLK/2);

	VK1024B_DATA_H();  
	WriteClockVk1024B();  //101
	VK1024B_DATA_L();  
	WriteClockVk1024B();
	VK1024B_DATA_H();  
	WriteClockVk1024B();

	for( i = 0; i < vk1024B_addrbit; i++) 
	{   		 
		if (Addr & Shift) 
			VK1024B_DATA_H(); 		
		else  
			VK1024B_DATA_L();
		WriteClockVk1024B();		
		Shift = Shift >> 1; 
	}
	Shift = 0x01;
	for (i = 0; i < 4; i++) 
	{

		if( Dat & Shift) 
			VK1024B_DATA_H();		 
   	else  
			VK1024B_DATA_L();
		WriteClockVk1024B();
		Shift = Shift << 1;
	}   
	VK1024B_CS_H(); //CS 片选关
	Vk1024B_Delay(VK1024B_CLK/2);	
	VK1024B_DATA_H(); 
}
/**
  * @brief  Write n data to Vk1024B 
  * @param  Addr->写入ram的起始地址 
  * @param  Databuf->写入ram的数据buffer
  * @param  Cnt->写入ram的数据个数
  * @retval None
  */
void WritenDataVk1024B(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char i,j; 
	unsigned char Shift;
	unsigned char dataval; 
	
	VK1024B_CS_L();   //CS 片选开 
	Vk1024B_Delay(VK1024B_CLK/2);

	VK1024B_DATA_H();  
	WriteClockVk1024B();  //101
	VK1024B_DATA_L();  
	WriteClockVk1024B();
	VK1024B_DATA_H();  
	WriteClockVk1024B();

	Shift = 0x20;
	for( i = 0; i < vk1024B_addrbit; i++) 
	{   		 
		if (Addr & Shift) 
			VK1024B_DATA_H(); 		
		else  
			VK1024B_DATA_L();
		WriteClockVk1024B();		
		Shift = Shift >> 1; 
	}
	for (j = 0; j < Cnt; j++) 
	{
		Shift = 0x01;
		dataval=*Databuf++;
		for (i = 0; i < 4; i++) 
		{
			if( dataval & Shift) 
				VK1024B_DATA_H();		 
			else  
				VK1024B_DATA_L();
			WriteClockVk1024B();
			Shift = Shift << 1;
		}   
	}
	VK1024B_CS_H();   //CS 片选关
	Vk1024B_Delay(VK1024B_CLK/2);	 
	VK1024B_DATA_H(); 
}

unsigned char CheckDisAll(u8 displays)
{
	if(displays)
	{
		if(Display_Sta!=0xffffffff)
		{
			Display_Sta=0xffffffff;
			return 1;
		}
		else return 0;
	}
	else
	{
		if(Display_Sta != 0)
		{
			Display_Sta=0;
			return 1;
		}
		else return 0;
	}
}

unsigned char CheckDisplayOn(u8 displays)
{
	if(!(Display_Sta&(1<<displays)))
	{
		Display_Sta |= (1<<displays);
		return 1;
	}else return 0;
}
unsigned char CheckDisplayOff(u8 displays)
{
	if((Display_Sta&(1<<displays)))
	{
		Display_Sta &= ~(1<<displays);
		return 1;
	}else return 0;
}

/**
  * @brief  lcd全显或全灭
  * @param  state=1->lcd全亮 
  * @param  state=0->lcd全灭
  * @retval None
  */
void Vk1024B_DisAll(unsigned char state)
{
	unsigned char segi;
	if(CheckDisAll(state))
	{
		for(segi=0;segi<VK1024B_SEGNUM;segi++)
		{
			if(state==1)
			{
				if(vk1024B_maxcom==2)
					vk1024B_dispram[segi]=0x03;
				else if(vk1024B_maxcom==3)
					vk1024B_dispram[segi]=0x07;
				else//maxcom==4
					vk1024B_dispram[segi]=0x0f;
			}
			else
			{
				vk1024B_dispram[segi]=0x00;
			}
		}
		WritenDataVk1024B(vk1024B_segtab[0],vk1024B_dispram,VK1024B_SEGNUM);
	}
}


/**
  * @brief  lcd显示单个点（1comx1seg）
  * @param  Seg->点对应的seg 
  * @param  Com->点对应的com
  * @retval None
  */
void Vk1024B_DisSegOn(unsigned char Seg,unsigned char Com)
{
	if(CheckDisplayOn((Com * 6) + Seg - 20))
	{
		vk1024B_dispram[Seg-vk1024B_segtab[0]]|=(1<<Com);
		Write1DataVk1024B(Seg,vk1024B_dispram[Seg-vk1024B_segtab[0]]);
	}
}
/**
  * @brief  lcd关闭单个点（1comx1seg）
  * @param  None 
  * @retval None
  */
void Vk1024B_DisSegOff(unsigned char Seg,unsigned char Com)
{
	if(CheckDisplayOff((Com * 6) + Seg - 20))
	{
		vk1024B_dispram[Seg-vk1024B_segtab[0]]&=~(1<<Com);
		Write1DataVk1024B(Seg,vk1024B_dispram[Seg-vk1024B_segtab[0]]);
	}
}
/**
  * @brief  配置Vk1024B通信线GPIO.
  * @param  None 
  * @retval None
  */
void Vk1024B_Lowlevel_Init(void)
{
	//通信线电平不同，建议加电平转换电路
	//此函数根据客户单片机做相应的修改
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = VK1024B_CS_PIN; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //通信线电平一样，可设为推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	//GPIO_OType_OD 通讯线电平不同，可设为开漏输出，1024B有内部上拉电阻
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(VK1024B_CS_GPIO_PORT,&GPIO_InitStructure); 		

	GPIO_InitStructure.GPIO_Pin =VK1024B_WR_PIN;	
	GPIO_Init(VK1024B_WR_GPIO_PORT, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin =VK1024B_DAT_PIN;	
	GPIO_Init(VK1024B_WR_GPIO_PORT, &GPIO_InitStructure);  	
}
/**
  * @brief  Vk1024B进入掉电低功耗模式
  * @param  None 
  * @retval None
  */
void Vk1024B_Enter_Standby(void)
{	
	//连续使用命令LCDOFF和SYSDIS,系统将处于掉电低功耗状态。
	//只有使用片内RC时钟源时,才能使系统进入掉电低功耗状态。
	WriteCommandVk1024B(OSC_OFF);
	WriteCommandVk1024B(DISP_OFF);
}
/**
  * @brief  Vk1024B退出掉电低功耗模式
  * @param  None 
  * @retval None
  */
void Vk1024B_Exit_Standby(void)
{	
	//退出掉电低功耗状态重新配置vk1024B
	WriteCommandVk1024B(OSC_ON);
	WriteCommandVk1024B(DISP_ON);
//	WriteCommandVk1024B(COM_1_3__4);vk1024B_maxcom=4;	//1/3bias 4com
//	WriteCommandVk1024B(COM_1_3__3);vk1024B_maxcom=3;	//1/3bias 3com
//	WriteCommandVk1024B(COM_1_3__2);vk1024B_maxcom=2;	//1/3bias 2com
	WriteCommandVk1024B(COM_1_2__4);vk1024B_maxcom=4;	//1/2bias 4com
//	WriteCommandVk1024B(COM_1_2__3);vk1024B_maxcom=3;	//1/2bias 3com
//	WriteCommandVk1024B(COM_1_2__2);vk1024B_maxcom=2;	//1/2bias 2com
}
/**
  * @brief  Vk1024B Init
  * @param  None 
  * @retval None
  */
void Vk1024B_Init(void)
{	
	//管脚配置根据客户单片机做相应的修改
	Vk1024B_Lowlevel_Init();
	//基本配置
	WriteCommandVk1024B(OSC_ON);
	WriteCommandVk1024B(DISP_ON);
//	WriteCommandVk1024B(COM_1_3__4);vk1024B_maxcom=4; 	// 1/3bias 4com
//	WriteCommandVk1024B(COM_1_3__3);vk1024B_maxcom=3;	//1/3bias 3com
//	WriteCommandVk1024B(COM_1_3__2);vk1024B_maxcom=2;	//1/3bias 2com
	WriteCommandVk1024B(COM_1_2__4);vk1024B_maxcom=4;	//1/2bias 4com
//	WriteCommandVk1024B(COM_1_2__3);vk1024B_maxcom=3;	//1/2bias 3com
//	WriteCommandVk1024B(COM_1_2__2);vk1024B_maxcom=2;	//1/2bias 2com
	//上电默认配置(以下未用功能关闭降低功耗)
	WriteCommandVk1024B(TIMER_DIS);
	WriteCommandVk1024B(WDT_DIS);
	WriteCommandVk1024B(BUZZ_OFF);
	WriteCommandVk1024B(IRQ_DIS);
	Vk1024B_DisAll(0);									//LCD全关
	Vk1024B_Enter_Standby();
}
/**
  * @brief  Vk1024B测试主程序
  * @param  None 
  * @retval None
  */
void Vk1024B_TEST(void)
{	
	Vk1024B_Init();
	Vk1024B_DisAll(0);
	while(1)
	{
		Vk1024B_DisAll(1);										//LCD全显
		Vk1024B_Delay(50*VK1024B_BASE100MS);	//延时5S
		Vk1024B_DisAll(0);										//LCD全关
		Vk1024B_Delay(50*VK1024B_BASE100MS);	//延时5S
		for(vk1024B_segi=0;vk1024B_segi<VK1024B_SEGNUM;vk1024B_segi++)//seg
		{
			for(vk1024B_comi=0;vk1024B_comi<vk1024B_maxcom;vk1024B_comi++)//com
			{
				Vk1024B_DisSegOn(vk1024B_segtab[vk1024B_segi],vk1024B_comi);	//LCD单个seg点亮
				Vk1024B_Delay(3*VK1024B_BASE100MS);				//延时300mS
				Vk1024B_DisAll(0);								//LCD全关
			}
		}
		Vk1024B_DisAll(1);										//LCD全显
		Vk1024B_Delay(10*VK1024B_BASE100MS);	//延时1S
		for(vk1024B_segi=0;vk1024B_segi<VK1024B_SEGNUM;vk1024B_segi++)//seg
		{
			for(vk1024B_comi=0;vk1024B_comi<vk1024B_maxcom;vk1024B_comi++)//com
			{
				Vk1024B_DisSegOff(vk1024B_segtab[vk1024B_segi],vk1024B_comi);	//LCD单个seg关闭
				Vk1024B_Delay(3*VK1024B_BASE100MS);	//延时300mS
				Vk1024B_DisAll(1);									//LCD全显
			}
		}
		Vk1024B_Delay(10*VK1024B_BASE100MS);//延时1S
		Vk1024B_DisAll(0);									//LCD全关
		Vk1024B_Delay(10*VK1024B_BASE100MS);//延时1S
		Vk1024B_Enter_Standby();						//Vk1024B进入掉电低功耗模式
		Vk1024B_Delay(50*VK1024B_BASE100MS);//延时5S
		Vk1024B_Exit_Standby();						//Vk1024B退出掉电低功耗模式
	}
}
/************************END OF FILE****/
