/**
  ******************************************************************************

  ******************************************************************************
  ******************************************************************************
  */	
/* Includes ------------------------------------------------------------------*/
#include "vk1024b.h"
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VK1024B_BASE100MS	226757 	//ͨ��n��nopָ��ʵ��100mS��ʱ�����ݵ�Ƭ���޸Ĵ˳���
#define VK1024B_CLK			10		//WR�ź���Ƶ�� ,��Vk1024B_Delayʵ�֣� =10->100kHz =100->11.11kHz
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const unsigned char vk1024B_segtab[VK1024B_SEGNUM]={
	19,20,21,22,23,24
};
unsigned char vk1024B_addrbit=6;//����ic��ram��ַ����A5-A0,
unsigned char vk1024B_segi,vk1024B_comi;
unsigned char vk1024B_maxcom=4;//������com��������4com��3com��2com
unsigned char vk1024B_dispram[VK1024B_SEGNUM];//��ʾRAM bufferֻʹ�õ�4λ��bit0->com0,bit1->com1,bit2->com2,bit3->com3
u32 Display_Sta=0x80000000;
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  Inserts a delay time.
  * @param  ucDat-> ��ʱʱ��
  * @retval None
  */
void Vk1024B_Delay(unsigned int ucDat)
{//ucDat=10->100kHz ucDat=100->11.11kHz ucDat=226757->100mS
	while(ucDat--) 
	{//nopָ����ݵ�Ƭ������Ӧ���޸�
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
  * @param  FunctonCode->����/������ 
  * @retval None
  */
void WriteCommandVk1024B(unsigned char FunctonCode)
{
	unsigned char Shift = 0x80; 
	unsigned char i;

	VK1024B_CS_L();   //CS Ƭѡ��   
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
	WriteClockVk1024B();	//����һ�� 0 16xx�����һλ X
	VK1024B_CS_H(); 			//CS Ƭѡ��  
	Vk1024B_Delay(VK1024B_CLK/2);
	VK1024B_DATA_H(); 
}
/**
  * @brief  Write 1 data to Vk1024B 
  * @param  Addr->д��ram�ĵ�ַ 
  * @param  Databuf->д��ram������
  * @retval None
  */
void Write1DataVk1024B(unsigned char Addr,unsigned char Dat)
{
	unsigned char i; 
	unsigned char Shift;
	Shift = 0x20; //���θ�3λ ֻ��5λ
	VK1024B_CS_L();  //CS Ƭѡ��
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
	VK1024B_CS_H(); //CS Ƭѡ��
	Vk1024B_Delay(VK1024B_CLK/2);	
	VK1024B_DATA_H(); 
}
/**
  * @brief  Write n data to Vk1024B 
  * @param  Addr->д��ram����ʼ��ַ 
  * @param  Databuf->д��ram������buffer
  * @param  Cnt->д��ram�����ݸ���
  * @retval None
  */
void WritenDataVk1024B(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char i,j; 
	unsigned char Shift;
	unsigned char dataval; 
	
	VK1024B_CS_L();   //CS Ƭѡ�� 
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
	VK1024B_CS_H();   //CS Ƭѡ��
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
		if(Display_Sta)
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
  * @brief  lcdȫ�Ի�ȫ��
  * @param  state=1->lcdȫ�� 
  * @param  state=0->lcdȫ��
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
  * @brief  lcd��ʾ�����㣨1comx1seg��
  * @param  Seg->���Ӧ��seg 
  * @param  Com->���Ӧ��com
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
  * @brief  lcd�رյ����㣨1comx1seg��
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
  * @brief  ����Vk1024Bͨ����GPIO.
  * @param  None 
  * @retval None
  */
void Vk1024B_Lowlevel_Init(void)
{
	//ͨ���ߵ�ƽ��ͬ������ӵ�ƽת����·
	//�˺������ݿͻ���Ƭ������Ӧ���޸�
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = VK1024B_CS_PIN; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //ͨ���ߵ�ƽһ��������Ϊ�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	//GPIO_OType_OD ͨѶ�ߵ�ƽ��ͬ������Ϊ��©�����1024B���ڲ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;	
	GPIO_Init(VK1024B_CS_GPIO_PORT,&GPIO_InitStructure); 		

	GPIO_InitStructure.GPIO_Pin =VK1024B_WR_PIN;	
	GPIO_Init(VK1024B_WR_GPIO_PORT, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin =VK1024B_DAT_PIN;	
	GPIO_Init(VK1024B_WR_GPIO_PORT, &GPIO_InitStructure);  	
}
/**
  * @brief  Vk1024B�������͹���ģʽ
  * @param  None 
  * @retval None
  */
void Vk1024B_Enter_Standby(void)
{	
	//����ʹ������LCDOFF��SYSDIS,ϵͳ�����ڵ���͹���״̬��
	//ֻ��ʹ��Ƭ��RCʱ��Դʱ,����ʹϵͳ�������͹���״̬��
	WriteCommandVk1024B(OSC_OFF);
	WriteCommandVk1024B(DISP_OFF);
}
/**
  * @brief  Vk1024B�˳�����͹���ģʽ
  * @param  None 
  * @retval None
  */
void Vk1024B_Exit_Standby(void)
{	
	//�˳�����͹���״̬��������vk1024B
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
	//�ܽ����ø��ݿͻ���Ƭ������Ӧ���޸�
	Vk1024B_Lowlevel_Init();
	//��������
	WriteCommandVk1024B(OSC_ON);
	WriteCommandVk1024B(DISP_ON);
//	WriteCommandVk1024B(COM_1_3__4);vk1024B_maxcom=4; 	// 1/3bias 4com
//	WriteCommandVk1024B(COM_1_3__3);vk1024B_maxcom=3;	//1/3bias 3com
//	WriteCommandVk1024B(COM_1_3__2);vk1024B_maxcom=2;	//1/3bias 2com
	WriteCommandVk1024B(COM_1_2__4);vk1024B_maxcom=4;	//1/2bias 4com
//	WriteCommandVk1024B(COM_1_2__3);vk1024B_maxcom=3;	//1/2bias 3com
//	WriteCommandVk1024B(COM_1_2__2);vk1024B_maxcom=2;	//1/2bias 2com
	//�ϵ�Ĭ������(����δ�ù��ܹرս��͹���)
	WriteCommandVk1024B(TIMER_DIS);
	WriteCommandVk1024B(WDT_DIS);
	WriteCommandVk1024B(BUZZ_OFF);
	WriteCommandVk1024B(IRQ_DIS);
	Vk1024B_DisAll(0);									//LCDȫ��
	Vk1024B_Enter_Standby();
}
/**
  * @brief  Vk1024B����������
  * @param  None 
  * @retval None
  */
void Vk1024B_TEST(void)
{	
	Vk1024B_Init();
	Vk1024B_DisAll(0);
	while(1)
	{
		Vk1024B_DisAll(1);										//LCDȫ��
		Vk1024B_Delay(50*VK1024B_BASE100MS);	//��ʱ5S
		Vk1024B_DisAll(0);										//LCDȫ��
		Vk1024B_Delay(50*VK1024B_BASE100MS);	//��ʱ5S
		for(vk1024B_segi=0;vk1024B_segi<VK1024B_SEGNUM;vk1024B_segi++)//seg
		{
			for(vk1024B_comi=0;vk1024B_comi<vk1024B_maxcom;vk1024B_comi++)//com
			{
				Vk1024B_DisSegOn(vk1024B_segtab[vk1024B_segi],vk1024B_comi);	//LCD����seg����
				Vk1024B_Delay(3*VK1024B_BASE100MS);				//��ʱ300mS
				Vk1024B_DisAll(0);								//LCDȫ��
			}
		}
		Vk1024B_DisAll(1);										//LCDȫ��
		Vk1024B_Delay(10*VK1024B_BASE100MS);	//��ʱ1S
		for(vk1024B_segi=0;vk1024B_segi<VK1024B_SEGNUM;vk1024B_segi++)//seg
		{
			for(vk1024B_comi=0;vk1024B_comi<vk1024B_maxcom;vk1024B_comi++)//com
			{
				Vk1024B_DisSegOff(vk1024B_segtab[vk1024B_segi],vk1024B_comi);	//LCD����seg�ر�
				Vk1024B_Delay(3*VK1024B_BASE100MS);	//��ʱ300mS
				Vk1024B_DisAll(1);									//LCDȫ��
			}
		}
		Vk1024B_Delay(10*VK1024B_BASE100MS);//��ʱ1S
		Vk1024B_DisAll(0);									//LCDȫ��
		Vk1024B_Delay(10*VK1024B_BASE100MS);//��ʱ1S
		Vk1024B_Enter_Standby();						//Vk1024B�������͹���ģʽ
		Vk1024B_Delay(50*VK1024B_BASE100MS);//��ʱ5S
		Vk1024B_Exit_Standby();						//Vk1024B�˳�����͹���ģʽ
	}
}
/************************END OF FILE****/
