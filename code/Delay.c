#include "Delay.h"
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

void Delay_Us(uint32 us)
{
    system_delay_us(us);
}

void Delay_Ms(uint16 ms)
{
    system_delay_ms(ms);
}



