/* 包含的头文件 --------------------------------------------------------------*/
#include "flash.h"


/* 变量 ----------------------------------------------------------------------*/
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

void FLASH_PageWrite(uint16_t *pBuffer, uint32_t WriteAddr)
{
	uint16_t cnt_tmp;
	for(cnt_tmp=0; (cnt_tmp<FLASH_PAGE_LENGTH) && (FLASHStatus == FLASH_COMPLETE); cnt_tmp++)
	{
		FLASHStatus = FLASH_ProgramHalfWord(WriteAddr, *pBuffer);
		WriteAddr += 2;
		pBuffer++;
	}
}

void FLASH_WriteNWord(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord)
{
	static uint16_t buf_tmp[FLASH_PAGE_LENGTH];
	uint32_t NbrOfPage = 0x00;                    
	uint32_t length_beyond_start;                
	uint32_t length_remain_start;              
	uint32_t addr_first_page;                  
	uint32_t addr_last_page;                  
	uint16_t *pbuf_tmp;                        
	uint16_t cnt_length;                          
	uint16_t cnt_page;                           
	uint32_t prog_addr_start;                  
	uint32_t length_beyond_last;                
	uint8_t  flag_last_page_fill;             

	length_beyond_start = ((WriteAddr % FLASH_PAGE_SIZE) / FLASH_TYPE_LENGTH);
	length_remain_start = FLASH_PAGE_LENGTH - length_beyond_start;
	addr_first_page     = WriteAddr - (WriteAddr % FLASH_PAGE_SIZE);

	if(0 == (length_beyond_start + nWord)%FLASH_PAGE_LENGTH)
	{
		flag_last_page_fill = FLAG_OK;            
		NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH;
	}
	else
	{
		flag_last_page_fill = FLAG_NOOK;          
		NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH + 1;
	}
	/* 解锁 */
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	FLASH_ReadNWord(buf_tmp, addr_first_page, FLASH_PAGE_LENGTH);
	FLASHStatus = FLASH_ErasePage(addr_first_page);
	if(1 == NbrOfPage)
	{
		pbuf_tmp = pBuffer;                        
		for(cnt_length=length_beyond_start; cnt_length<(length_beyond_start + nWord); cnt_length++)
		{
			buf_tmp[cnt_length] = *pbuf_tmp;
			pbuf_tmp++;
		}
		FLASH_PageWrite(buf_tmp, addr_first_page);
	}
	else
	{
		pbuf_tmp = pBuffer;
		for(cnt_length=length_beyond_start; cnt_length<FLASH_PAGE_LENGTH; cnt_length++)
		{
			buf_tmp[cnt_length] = *pbuf_tmp;
			pbuf_tmp++;
		}
		FLASH_PageWrite(buf_tmp, addr_first_page);

		if(FLAG_OK == flag_last_page_fill)
		{
			for(cnt_page=1; (cnt_page<NbrOfPage)  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
			{                                          
				prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
				FLASHStatus = FLASH_ErasePage(prog_addr_start);
				                                       
				FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
			}
		}
		else
		{
			for(cnt_page=1; (cnt_page<(NbrOfPage - 1))  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
			{                                       
				prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
				FLASHStatus = FLASH_ErasePage(prog_addr_start);
				FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
			}
			addr_last_page = addr_first_page + (NbrOfPage - 1)*FLASH_PAGE_SIZE;

			FLASH_ReadNWord(buf_tmp, addr_last_page, FLASH_PAGE_LENGTH);
			FLASHStatus = FLASH_ErasePage(addr_last_page);
			pbuf_tmp = pBuffer + length_remain_start + (NbrOfPage - 2)*(FLASH_PAGE_SIZE/2);
			length_beyond_last   = (nWord - length_remain_start) % FLASH_PAGE_LENGTH;
			for(cnt_length=0; cnt_length<length_beyond_last; cnt_length++)
			{
				buf_tmp[cnt_length] = *pbuf_tmp;
				pbuf_tmp++;
			}
			FLASH_PageWrite(buf_tmp, addr_last_page);
		}
	}
}

void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord)
{
	while(nWord--)
	{
		*pBuffer = (*(__IO uint16_t*)ReadAddr);
		ReadAddr += 2;
		pBuffer++;
	}
}


