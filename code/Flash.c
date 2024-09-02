#include "flash.h"
//#include "cpu0_main.h"
/*
********************************************************************************************************
*                                               示例代码
*                                             EXAMPLE  CODE
*
*                             (c) Copyright 2021; SaiShu.Lcc.; Leo
*                                 版权所属[北京赛曙科技有限公司]
*
*               The code is for internal use only, not for commercial transactions(开源学习,请勿商用).
*               The code ADAPTS the corresponding hardware circuit board(代码使用CarDo智控板),
*               the specific details consult the professional(欢迎联系我们).
*********************************************************************************************************
*/


bool flashSaveEnable;



/**
* @brief        存储系统配置（全部/ALL）
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
    flash_buffer_clear();   //逐飞清空缓存区

    memset(flash_union_buffer,0,sizeof(flash_union_buffer));

    /*1*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdMiddle;//舵机中值PWM
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(flash_union_buffer+(i++)*4,buff,4);

    /*2*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdLeft;//舵机左转阈值
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(flash_union_buffer+(i++)*4,buff,4);

    /*3*/       buff[0] = FLASH_DATA_OK;
                bint16_Union.U16 = servoStr.thresholdRight;//舵机右转阈值
                buff[1] = bint16_Union.U8_Buff[0];
                buff[2] = bint16_Union.U8_Buff[1];
                memcpy(flash_union_buffer+(i++)*4,buff,4);

    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // 向指定 Flash 扇区的页码写入缓冲区数据
}

/**
* @brief        加载系统配置（全部/ALL）
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
//    flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX); //读取Flash数据
//
//    /*1*/       memcpy(buff,flash_union_buffer+(i++)*4,4);
//                if(buff[0] == FLASH_DATA_OK)                //舵机中值PWM
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
//                if(buff[0] == FLASH_DATA_OK)                //舵机左转阈值
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
//                if(buff[0] == FLASH_DATA_OK)                //舵机右转阈值
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
* @brief        Flash存数据控制
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



