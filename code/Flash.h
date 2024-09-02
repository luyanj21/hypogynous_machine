#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"

//����
#define FLASH_DATA_OK           0x5F                                            //Flash�洢��־
//���
#define FLASH_SECTION_INDEX       (0)                                 // �洢�����õ�����
#define FLASH_PAGE_INDEX          (8)                                // �洢�����õ�ҳ�� ������һ��ҳ��



extern bool flashSaveEnable;



void FLASH_SaveAllConfig(void);
void FLASH_LoadAllConfig(void);
void FLASH_Handle(void);


#endif

//===========================================  End Of File  ===========================================//



