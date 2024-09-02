#include "flash.h"
//#include "cpu0_main.h"
/*
********************************************************************************************************
*                                               ʾ������
*                                             EXAMPLE  CODE
*
*                             (c) Copyright 2021; SaiShu.Lcc.; Leo
*                                 ��Ȩ����[��������Ƽ����޹�˾]
*
*               The code is for internal use only, not for commercial transactions(��Դѧϰ,��������).
*               The code ADAPTS the corresponding hardware circuit board(����ʹ��CarDo�ǿذ�),
*               the specific details consult the professional(��ӭ��ϵ����).
*********************************************************************************************************
*/


bool flashSaveEnable;



/**
* @brief        �洢ϵͳ���ã�ȫ��/ALL��
* @param
* @ref
* @author       Leo
* @note
**/
void FLASH_SaveAllConfig(void)
{
    int i = 0;
    uint8_t buff[4] = {0};
    Bint16_Union bint16_Union;
    flash_buffer_clear();   //�����ջ�����

    memset(flash_union_buffer,0,sizeof(flash_union_buffer));

    /*1*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdMiddle;//�����ֵPWM
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(flash_union_buffer+(i++)*4,buff,4);

    /*2*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdLeft;//�����ת��ֵ
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(flash_union_buffer+(i++)*4,buff,4);

    /*3*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdRight;//�����ת��ֵ
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(flash_union_buffer+(i++)*4,buff,4);

    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // ��ָ�� Flash ������ҳ��д�뻺��������
}

/**
* @brief        ����ϵͳ���ã�ȫ��/ALL��
* @param
* @ref
* @author       Leo
* @note
**/
//void FLASH_LoadAllConfig(void)
//{
//    int i = 0;
//    Bint16_Union bint16_Union;
//    uint8_t buff[4];
//
//    flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX); //��ȡFlash����
//
//    /*1*/       memcpy(buff,flash_union_buffer+(i++)*4,4);
//                if(buff[0] == FLASH_DATA_OK)                //�����ֵPWM
//                {
//                    bint16_Union.U8_Buff[0] = buff[1];
//                    bint16_Union.U8_Buff[1] = buff[2];
//                    servoStr.thresholdMiddle = bint16_Union.U16;
//                }
//                else
//                {
//                    servoStr.thresholdMiddle = SERVO_PWM_MIDDLE;
//                }
//
//    /*2*/       memcpy(buff,flash_union_buffer+(i++)*4,4);
//                if(buff[0] == FLASH_DATA_OK)                //�����ת��ֵ
//                {
//                    bint16_Union.U8_Buff[0] = buff[1];
//                    bint16_Union.U8_Buff[1] = buff[2];
//                    servoStr.thresholdLeft = bint16_Union.U16;
//                }
//                else
//                {
//                    servoStr.thresholdLeft = SERVO_PWM_MAX_L;
//                }
//
//    /*3*/       memcpy(buff,flash_union_buffer+(i++)*4,4);
//                if(buff[0] == FLASH_DATA_OK)                //�����ת��ֵ
//                {
//                    bint16_Union.U8_Buff[0] = buff[1];
//                    bint16_Union.U8_Buff[1] = buff[2];
//                    servoStr.thresholdRight = bint16_Union.U16;
//                }
//                else
//                {
//                    servoStr.thresholdRight = SERVO_PWM_MAX_R;
//                }
//}
void FLASH_LoadAllConfig(void)
{
     servoStr.thresholdMiddle = SERVO_PWM_MIDDLE;

     servoStr.thresholdLeft = SERVO_PWM_MAX_L;

     servoStr.thresholdRight = SERVO_PWM_MAX_R;
}





/**
* @brief        Flash�����ݿ���
* @param
* @ref
* @author       Leo
* @note
**/
void FLASH_Handle(void)
{
    if(flashSaveEnable)
    {
        FLASH_SaveAllConfig();
        flashSaveEnable = false;
    }
}



