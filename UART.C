/**********************************************************************
文件名称：uart.c
当前版本：
作者： 
完成日期： 
***********************************************************************/
#include "Uart.h"

u16 UART_RX_STA=0;
char UART_RX_BUF[UART_REC_LEN];
int32 RX_BUF[14] = {0};
p_Inquire pInquire;

void Uart_Init(uint32_t baud)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*使能GPIO, USART1时钟	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );	
	GPIO_PinAFConfig(PORT_UART,PINSOURCE_RX,GPIO_AF_0);	
	GPIO_PinAFConfig(PORT_UART,PINSOURCE_TX,GPIO_AF_0);       

	/*USART1端口配置  */
	GPIO_InitStructure.GPIO_Pin = PIN_RX ; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   		//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  			//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;		//速度50MHz
	GPIO_Init(PORT_UART,&GPIO_InitStructure); 			

	GPIO_InitStructure.GPIO_Pin =  PIN_TX; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   		//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 		
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  		
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;		//速度50MHz
	GPIO_Init(PORT_UART,&GPIO_InitStructure); 			

	USART_InitStructure.USART_BaudRate = baud;	//设置串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验位													
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	/*无硬件数据流控制*/
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//设置工作模式收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART1, ENABLE);//使能串口1
	USART_ClearFlag(USART1, USART_FLAG_TC);	

#if EN_USART1_RX
	/*开启相关中断*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	/*Usart1 NVIC 配置*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority=3 ;	//优先级3 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
#endif
}
void UART_SendData8(u8 data)
{
	while((USART1->ISR & USART_FLAG_TXE) == RESET);
	USART1->TDR = (data & (uint16_t)0x01FF);
}


void USART1_IRQHandler(void)                	
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); // 清中断标志
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if((UART_RX_STA & 0x8000) == 0)//接收未完成
		{
			UART_RX_Time=50;		//计时时间
			UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
			UART_RX_STA++;
			if(UART_RX_STA == (UART_REC_LEN - 1)) 
			{
				UART_RX_Time = 0;
				UART_RX_STA |= 0x8000;	//接收完成了 			
			}
		}   		 
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET) // 检查 ORE 标志
	{
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1,USART_FLAG_ORE);
	}
}


void USART1_ClearDate(void)
{
	u16 len;		
	len=UART_RX_STA&0x3fff;//得到此次接收到的数据长度
	memset(UART_RX_BUF,0,len);
	UART_RX_STA=0;
}

void USART1_Printf_Receive(void)
{
	u16  t,len;	
	if(UART_RX_STA&0x8000)
	{					   
		len=UART_RX_STA&0x3fff;//得到此次接收到的数据长度
		mPrintf(" The command is:\r\n");
		for(t=0;t<len;t++)
		{
			USART_SendData(USART1, UART_RX_BUF[t]);//向串口1发送数据
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		mPrintf("\r\n\r\n");
	}
}
// 从字符串 （只取连续的数字） 转换成 uint32 
int32_t str2int(char *str ) 
{ 
	int32_t temp = 0;
	char *ptr = str;  				//ptr保存str字符串开头
	if(*str == '-' || *str == '+') 	//如果第一个字符是正负号，
		str++; 					//则移到下一个字符
	while(*str != 0)
	{
		if ((*str < '0') || (*str > '9')) 	  //if(! isdigit(*str)) 如果当前字符不是数字
			break;                        	  //则退出循环
		temp = temp*10 + (*str - '0');    //如果当前字符是数字则计算数值
		str++;     					 //移到下一个字符 
	}
	if (*ptr == '-')       //如果字符串是以“-”开头，则转换成其相反数
		temp = -temp;  
	return temp; 
} 
//从字符串 提取 第n组_后的连续的数字 转换成 uint32
// 例   B3S_0_0_10636_3544_3535_3557_0_0_939_0_0_1929_2044_5a595c10
// 例   temp= str_int_n( strstr(g_RxBuf_DB,“B3S" ),3 );  得 10636
int32_t str_int_n( const char *str, uint8_t n )
{
	 int32_t temp = 0;
	 const char *ptr;//ptr保存str字符串开头
	 while(*str != 0)
	 {
		if (*str =='_' && n ) //  第n组_
		{ 
			n--; 
			if( n==0 ) 
				str++; 
		}
		if( n== 0 )
		{	
			if(*str == '-' || *str == '+' ) //如果第一个字符是正负号，
			{ 
				ptr = str;
				str++;  //则移到下一个字符
			}
			if ((*str < '0') || (*str > '9')) //如果当前字符不是数字
				break;                    //则退出循环
			temp = temp*10 + (*str - '0');    //如果当前字符是数字则计算数值
		}
		str++;      //移到下一个字符
	 }
	 if (*ptr == '-')       //如果字符串是以“-”开头，则转换成其相反数
	 { temp = -temp;  }
	 return temp;
}

int32_t str_int_M( const char *str, uint8_t n )
{
	 int32_t temp = 0;
	 const char *ptr;//ptr保存str字符串开头
	 while(*str != 0)
	 {
		if (*str =='M' && n ) //  第n组_
		{ 
			n--; 
			if( n==0 ) 
				str++; 
		}
		if( n== 0 )
		{	
			if(*str == '-' || *str == '+' ) //如果第一个字符是正负号，
			{ 
				ptr = str;
				str++;  //则移到下一个字符
			}
			if ((*str < '0') || (*str > '9')) //如果当前字符不是数字
				break;                    //则退出循环
			temp = temp*10 + (*str - '0');    //如果当前字符是数字则计算数值
		}
		str++;      //移到下一个字符
	 }
	 if (*ptr == '-')       //如果字符串是以“-”开头，则转换成其相反数
	 { temp = -temp;  }
	 return temp;
}

void Inquire(void)
{
	if(pInquire == Iq_B3S)	
	{
		RX_BUF[Counter]++;
		if(RX_BUF[Counter] > 1)
		{
			RX_BUF[Counter] = 0;
			if(State.Capy_Calculate)
			{
				State.Capy_Calculate = 0;
				printf("BMI=_1_%d_%d",RX_BUF[TrueCapy],RX_BUF[TrueCapy]);
			}
			else
			{
				printf(T_B3S_S);	//发送字符查询B3S
			}
		}
		pInquire = Iq_ACS;
	}
	else if(pInquire == Iq_ACS)	
	{
		printf(T_ACS_S);	//发送字符查询ACS
		pInquire = Iq_ONOFF;
	}
	else
	{
		if(State.Print_S)
			Printfstatus();
		if(State.AC_OUT_S)
		{
			if((RX_BUF[AcsState] == 99) ||(RX_BUF[DcsState] == 99))//还没开启
				printf(T_DCON_S);
		}
		else
		{
			if(RX_BUF[DcsState] != 99)
				printf(T_DCOFF_S);
		}
		pInquire = Iq_B3S;
	}
}

void ACSDCSB3S_State(void)
{
	const char *ptemp;
	const char *ptemp1;
	const char *ptemp2;
	if(UART_RX_STA & 0x8000)
	{
		if(ptemp = strstr(UART_RX_BUF,R_B3S_S))
		{
			RX_BUF[B3sState] = str_int_n(ptemp,2);
			RX_BUF[Volbuf] = str_int_n(ptemp,3);
			RX_BUF[NowCapy] = str_int_n(ptemp,10);
			RX_BUF[SOCbuf] = str_int_n(ptemp,11);
			RX_BUF[Cell_V1] = str_int_n(ptemp,4);
			RX_BUF[Cell_V2] = str_int_n(ptemp,5);
			RX_BUF[Cell_V3] = str_int_n(ptemp,6);
			RX_BUF[TrueCapy] = str_int_M(ptemp,1);
			
			B3S_RX_Time = B3S_OVTime;
			BuzzerBit.Data_Bat.BitBat.B3Sc_Err = 0;
			DisplayBit.Data_Bat.BitBat.B3Sc_Err = 0;
			
			if((RX_BUF[B3sState]==SAVE_NT_BT) 
			||(RX_BUF[B3sState]==SAVE_NT_FT))
			{
				if(!DisplayBit.Data_Bat.BitBat.BatPro)
				{
					BuzzerBit.Data_Bat.BitBat.BatPro = 1;
					DisplayBit.Data_Bat.BitBat.BatPro = 1;
				}
			}
			else
			{
				BuzzerBit.Data_Bat.BitBat.BatPro = 0;
				DisplayBit.Data_Bat.BitBat.BatPro = 0;
			}
			if(RX_BUF[B3sState]==SAVE_NT_BC) 
				State.Charge_P_S = 1;
			else
				State.Charge_P_S = 0;
			if(RX_BUF[B3sState]==SAVE_BT_HV)
				State.CH_Full_S = 1;
			
			if((RX_BUF[B3sState] == 1)
			&& (RX_BUF[NowCapy] > (RX_BUF[TrueCapy] + 1000))
			&& (RX_BUF[TrueCapy] > 50000)
			&& (RX_BUF[TrueCapy] < 65000))
				State.Capy_Calculate = 1;
			State.B3S_Finish_S = 1;
		}
		if(ptemp1 = strstr(UART_RX_BUF,R_DCS_S))
		{
			RX_BUF[DcsState]= str_int_n(ptemp1,2); 
			if((ptemp2 = strstr(UART_RX_BUF,R_ACS_S)) && (State.Key_S))
			{
				RX_BUF[AcsState] = str_int_n(ptemp2,2);
				RX_BUF[Powbuf] = str_int_n(ptemp2,3);
				RX_BUF[ACVol] = str_int_n(ptemp2,4);
			}
			else
			{
				RX_BUF[AcsState] = 0;
				RX_BUF[Powbuf] =0;
				RX_BUF[ACVol] = 0;
			}
			BuzzerBit.Data_Bat.BitBat.ACS_Err = 0;
			DisplayBit.Data_Bat.BitBat.ACS_Err = 0;
			DCS_RX_Time = DCS_OVTime;
			State.DCS_Finish_S = 1;
		}
		if(strstr(UART_RX_BUF,R_COM_S))
			State.Print_S = !State.Print_S;
		USART1_ClearDate();
	}
}




#if 1
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
/*定义_sys_exit()以避免使用半主机模式*/    
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{ 
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	return (ch);
} 
#endif
#if 0

/*系统printf函数实现*/
int putchar(int c)
{ 
	if('\n' == (char)c) 
	{   
		UART_SendData8('\r');    
		while((UART2_SR & 0x80)==0x00);
	} 
	UART_SendData8(c);  
	while((UART2_SR & 0x80)==0x00);
	return (c);
}
#endif

#if 0

/*  功能：将int型数据转为2，8，10，16进制字符串  
参数：value --- 输入的int整型数    
			str --- 存储转换的字符串    
			radix --- 进制类型选择  
注意：8位单片机int字节只占2个字节*/
char *sky_itoa(int value, char *str, unsigned int radix)
{ 
	char list[] = "0123456789ABCDEF"; 
	unsigned int tmp_value; 
	int i = 0, j, k = 0; 
	if (NULL == str) 
	{  
		return NULL;  
	}
	if (2 != radix && 8 != radix && 10 != radix && 16 != radix)
	{  
		return NULL; 
	}  
	if (radix == 10 && value < 0) 

	{    //十进制且为负数    
		tmp_value = (unsigned int)(0 - value);   
		str[i++] = '-';   
		k = 1;
	} 
	else 
	{  
		tmp_value = (unsigned int)value; 
	}  //数据转换为字符串，逆序存储  
	do 
	{  
		str[i ++] = list[tmp_value%radix];  
		tmp_value /= radix; 
	} 
	while(tmp_value); 
	str[i] = '\0';  //将逆序字符串转换为正序 
	char tmp;  
	for (j = k; j < (i+k)/2; j++) 
	{    
		tmp = str[j];  
		str[j] = str[i-j-1+k];  
		str[i-j-1+k] = tmp; 
	} 
	return str;
} 
/*  功能：将double型数据转为字符串  
参数：value --- 输入的double浮点数        
			str --- 存储转换的字符串        
			eps --- 保留小数位选择，
至少保留一个小数位,
至多保留4个小数位  
注意：8位单片机int字节只占2个字节*/
void sky_ftoa(double value, char *str, unsigned int eps)
{  
	unsigned int integer;
	double decimal;  
	char list[] = "0123456789";  
	int i = 0, j, k = 0;  //将整数及小数部分提取出来 
	if (value < 0)
	{   
		decimal = (double)(((int)value) - value);    
		integer = (unsigned int)(0 - value);  
		str[i ++] = '-';   
		k = 1;  
	} 
	else 
	{   
		integer = (unsigned int)(value);  
		decimal = (double)(value - integer); 
	}  //整数部分数据转换为字符串，逆序存储 
	do 
	{  
		str[i ++] = list[integer%10];  
		integer /= 10;  
	} 
	while(integer);  
	str[i] = '\0';  //将逆序字符串转换为正序 
	char tmp; 
	for (j = k; j < (i+k)/2; j++) 
	{   
		tmp = str[j];
		str[j] = str[i-j-1+k];
		str[i-j-1+k] = tmp;
	}  //处理小数部分 
	if (eps < 1 || eps > 4) 
	{   
		eps = 4; 
	}    
	//精度问题，防止输入1.2输出1.19等情况  
	double pp = 0.1; 
	for (j = 0; j <= eps; j++)
	{   
		pp *= 0.1;  
	}
	decimal += pp; 
	while (eps) 
	{    
		decimal *= 10;   
		eps --; 
	} 
	int tmp_decimal = (int)decimal;  
	str[i ++] = '.';
	k = i;  
	//整数部分数据转换为字符串，逆序存储 
	do
	{   
		str[i ++] = list[tmp_decimal%10];   
		tmp_decimal /= 10; 
	} 
	while(tmp_decimal); 
	str[i] = '\0';  
	//将逆序字符串转换为正序  
	for (j = k; j < (i+k)/2; j++) 
	{  
		tmp = str[j];  
		str[j] = str[i-j-1+k]; 
		str[i-j-1+k] = tmp;  
	}  
	str[i] = '\0';
} 

void mPrintf(char * Data, ...)
{ 
	const char *s;  
	int d;    
	char buf[16]; 
	u8 txdata; 
	double num;
	va_list ap;  
	va_start(ap, Data);
	while ( * Data != 0 ) 
	{	
		if ( * Data == 0x5c )  
		{									 
			switch ( *++Data ) 
			{     
			case 'r':							           
				txdata = 0x0d;     
				UART_SendData8(txdata);    
				Data ++;     
				break;     
			case 'n':			
				txdata = 0x0a;    
				UART_SendData8(txdata);        
				Data ++;        
				break;    
			default:        
				Data ++;     
				break;   
			}			  
		}
		else if ( * Data == '%') 
		{									      
			switch ( *++Data )
			{				 
			case 's':		
				s = va_arg(ap, const char *);       
				for ( ; *s; s++)
				{        
				UART_SendData8(*((u8 *)s));      
				}				      
				Data++;				    
				break;     
			case 'd':			     
				d = va_arg(ap, int);					
				sky_itoa(d, buf, 10);					    
				for (s = buf; *s; s++)
				{        
					UART_SendData8(*((u8 *)s));   
				}					      
				Data++;	
				break;     
			case 'x': 
				d = va_arg(ap, int);			
				sky_itoa(d, buf, 16);				
				for (s = buf; *s; s++) 
				{    
					UART_SendData8(*((u8 *)s));      
				}					        
				Data++;		
				break;     
			case 'f':
				num = va_arg(ap, double);	
				sky_ftoa(num, buf, 4);      
				for (s = buf; *s; s++) 
				{  
					UART_SendData8(*((u8 *)s));    
				}					      
				Data++;			       
				break;  
			default:       
				Data++;				        
				break;	
			}		  
		} 
		else
		{     
			UART_SendData8(*((u8 *)Data));    
			Data++;    
		}  
	}
}

#endif
