#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"

//赛曙
#define FLASH_DATA_OK           0x5F                                            //Flash存储标志
//逐飞
#define FLASH_SECTION_INDEX       (0)                                 // 存储数据用的扇区
#define FLASH_PAGE_INDEX          (8)                                // 存储数据用的页码 倒数第一个页码



extern bool flashSaveEnable;



void FLASH_SaveAllConfig(void);
void FLASH_LoadAllConfig(void);
void FLASH_Handle(void);


#endif

//===========================================  End Of File  ===========================================//



