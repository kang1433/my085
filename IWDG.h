


/**********************************  STM32F0xx  *********************************
  * @文件名	： IWDG.h
  * @日期		： 2018年09月19日
  ******************************************************************************/


#ifndef __IWDG_H
#define __IWDG_H
#include "include.h"

#define FeedDogTime 1000 //ms

void WDG_Config(void);
void Feed_Dog(void);


#endif


