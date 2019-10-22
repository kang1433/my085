#ifndef _LCD_h
#define _LCD_h
#include "include.h"


/*------COM1--------*/
/*充电左/20%/out框/放电下/USB */
#define	COM1					1				//PE2	
#define  	LCD_C_LEFT					20,0		
#define  	LCD_BAT_20					21,0		
#define  	LCD_FRAME_OUT				22,0	
#define  	LCD_D_DOWN				23,0			
#define  	LCD_USB					24,0		

/*------COM2--------*/
/*市电  ，充电下，40%，100%  ，放电左，12V*/
#define	COM2					2				//PG1	
#define  	LCD_AC_CH					19,1		
#define  	LCD_C_DOWN				20,1
#define  	LCD_BAT_40					21,1
#define  	LCD_BAT_100				22,1
#define  	LCD_D_LEFT					23,1
#define  	LCD_DC12					24,1

/*------COM3--------*/						//PG0	
/*ADC_V_Solar   ，充电上，60%，电池框，放电右，点烟*/
#define	COM3					3
#define  	LCD_SUN_CH					19,2
#define  	LCD_C_UP					20,2
#define  	LCD_BAT_60					21,2
#define  	LCD_FRAME_BAT				22,2
#define  	LCD_D_RIGHT				23,2
#define  	LCD_SMOKE					24,2

/*------COM4--------*/						//PE5	
/*int框 ，充电右，80%，报警  ，放电上，AC*/
#define	COM4					4
#define  	LCD_FRAME_IN				19,3
#define  	LCD_C_RIGHT				20,3
#define  	LCD_BAT_80					21,3
#define  	LCD_WARNING				22,3
#define  	LCD_D_UP					23,3
#define  	LCD_AC_Out					24,3




void DisplayTask(void);
void DisplayWaterfalLight(void);
void LCD_Init(void);
void LCD_SHOW(void);
void COM_POSISHOW(u8 COM, u8 SEG);
void COM_NEGASHOW(u8 COM, u8 SEG);
void COM_off(void);
void Running_light(void);




#endif
