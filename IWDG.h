


/**********************************  STM32F0xx  *********************************
  * @�ļ���	�� IWDG.h
  * @����		�� 2018��09��19��
  ******************************************************************************/


#ifndef __IWDG_H
#define __IWDG_H
#include "include.h"

#define FeedDogTime 1000 //ms

void WDG_Config(void);
void Feed_Dog(void);


#endif


