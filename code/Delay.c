#include "Delay.h"
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

void Delay_Us(uint32 us)
{
    system_delay_us(us);
}

void Delay_Ms(uint16 ms)
{
    system_delay_ms(ms);
}



