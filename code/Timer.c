#include "Timer.h"
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


/**
* @brief        定时器TIM2初始化
* @param        
* @ref          
* @author       Leo
* @note         
**/
void Timer_Init(void)
{
    pit_ms_init(PIT_NUM, 1);  // 初始化 CCU6_0_CH0 为周期中断 1ms 周期
}


/**
* @brief        CCU61_CH0定时中断服务
* @param        
* @ref          
* @author       Leo
* @note         
**/
//luy因为蜂鸣器鸣叫时间是和这个中断挂钩的，所以必须要1ms一次
IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU61_CH0);

//1.
/*luy 对蜂鸣器一个timer里面叫的时间计数，叫完标志是cut<counter*/
    GPIO_Timer();           //GPIO外设线程

//2.
//luy 编码器采样
//luy 测试使能||上位机连接（开闭环（电机转））
    MOTOR_Timer();          //电机控制线程


//3.
//    SOC_Timer();            //电量计监测线程

//4.
/*luy闭环冲刺和案件计时*/
    ICAR_Timer();           //智能车综合处理线程计数器


//5.
/*luy 掉线检测*/
    USB_Edgeboard_Timr();   //USB通信线程
}












