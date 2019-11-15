/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _FLASH_H
#define _FLASH_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "include.h"
/* 宏定义 --------------------------------------------------------------------*/
/*0X0800F000 - 0X0800F3FF*/
/*0X0800F400 - 0X0800F7FF*/
/*0X0800F800 - 0X0800FBFF*/
/*0X0800FC00 - 0X0800FFFF*/
#define FLASH_ADDR_FLAG           0x0800F000     //FLASH地址 - 标志位
#define FLASH_ADDR_DATA          0x0800F002     //FLASH地址 - 数据


/* 类型定义 ------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* 宏定义 --------------------------------------------------------------------*/
#define FLASH_PAGE_SIZE           ((uint16_t)0x400)         //页大小   - 1K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)         //类型大小 - 2字节
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01

/* 函数申明 ------------------------------------------------------------------*/
void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);


#endif /* _FLASH_H */

