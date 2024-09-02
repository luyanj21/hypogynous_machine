#include "Gpio.h"
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
//发车按键的外部中断:使用通道四 端口位33.7 ERU_CH4_REQ8_P33_7
//中断服务函数



GpioStruct gpioStr;
BuzzerStruct buzzerStr;

/**
* @brief        GPIO外设初始化
* @param
* @ref
* @author       Leo
* @note         外设：蜂鸣器 x1  状态LED x1  按键输入 x1
**/
void GPIO_Initialize(void)
{
    /*-----------------------------------------LED-----------------------------------------*/
    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // 初始化 LED1 输出 默认高电平 推挽输出模式
    /*-----------------------------------------按键-----------------------------------------*/
    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY1 输入 默认高电平 上拉输入
    /*-----------------------------------------蜂鸣器-----------------------------------------*/
    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);                        // 初始化 BUZZER_PIN 输出 默认低电平 推挽输出模式
    //上升沿触发
    exti_init(EXTI_PORT1, EXTI_TRIGGER_BOTH);             // 使用的外部中断输入引脚

    //BuzzerInit
    buzzerStr.Counter = 0;
    buzzerStr.Cut = 0;
    //luyanj:蜂鸣器使能改为开
    buzzerStr.Enable = true;
    //luyanj:沉默使能关
    buzzerStr.Silent = false;
    buzzerStr.Times = 0;
}


//----------------------------------------------[UNIT-按键及数字输入中断]----------------------------------------------------------

/**
* @brief        按键A中断响应函数
* @param
* @ref
* @author       Leo
* @note
**/
//中断优先级：抢占和子优先级都为1
 IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    if(exti_flag_get(EXTI_PORT1)==1)
    {
        if(!gpio_get_level(KEY1))    //按键按下
        {
            //luyanj:
//            ips114_show_string( 0 , 16*2,   "key_pressing");                            // 显示字符串
            gpioStr.KeyPress = true;
            GPIO_BuzzerEnable(BuzzerDing);
        }
        else    //按键弹起
        {
            gpioStr.KeyPress = false;
            //luyanj:
//            ips114_show_string( 0 , 16*2,   "key_released");                            // 显示字符串
        }
    }
    exti_flag_clear(EXTI_PORT1);
}




// IFX_INTERRUPT(exti_ch1_ch5_isr, 0, EXTI_CH1_CH5_INT_PRIO)
// {
//     interrupt_global_enable(0);                     // 开启中断嵌套
//     if(exti_flag_get(ERU_CH1_REQ10_P14_3))          // 通道1中断
//     {
//         exti_flag_clear(ERU_CH1_REQ10_P14_3);
//     }
//
//     if(exti_flag_get(EXTI_PORT1))           // 通道5中断
//     {
//         exti_flag_clear(EXTI_PORT1);
//         if(!gpio_get_level(KEY1))    //按键按下
//         {
//             //luyanj:
//             ips114_show_string( 0 , 16*2,   "key_pressing");                            // 显示字符串
//             gpioStr.KeyPress = true;
////             GPIO_BuzzerEnable(BuzzerDing);
//         }
//         else    //按键弹起
//         {
//             gpioStr.KeyPress = false;
//             //luyanj:
//             ips114_show_string( 0 , 16*2,   "key_released");                            // 显示字符串
//         }

//     }
// }

//IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
//{
//    interrupt_global_enable(0);                     // 开启中断嵌套
//    if(exti_flag_get(ERU_CH3_REQ6_P02_0))           // 通道3中断
//    {
//        exti_flag_clear(ERU_CH3_REQ6_P02_0);
//        camera_vsync_handler();                     // 摄像头触发采集统一回调函数
//    }
//    if(exti_flag_get(EXTI_PORT1))          // 通道7中断
//    {
//        exti_flag_clear(EXTI_PORT1);
//               if(!gpio_get_level(KEY1))    //按键按下
//               {
//                   //luyanj:
//                   ips114_show_string( 0 , 16*2,   "key_pressing");                            // 显示字符串
//                   gpioStr.KeyPress = true;
//      //             GPIO_BuzzerEnable(BuzzerDing);
//               }
//               else    //按键弹起
//               {
//                   gpioStr.KeyPress = false;
//                   //luyanj:
//                   ips114_show_string( 0 , 16*2,   "key_released");                            // 显示字符串
//               }
//
//
//
//
//
//    }
//}
//------------------------------------------------[END]-------------------------------------------------------------




/**
* @brief        GPIO线程控制器
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Timer(void)
{
    //蜂鸣器控制
    if(buzzerStr.Enable)
    {
        buzzerStr.Counter++;
//luy 不懂，八成是防止溢出的
        if(buzzerStr.Cut<buzzerStr.Counter)
            buzzerStr.Counter = buzzerStr.Cut;
    }

    //LED闪烁
    gpioStr.CounterLed++;
}



/**
* @brief        GPIO逻辑处理函数
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Handle(void)
{
    //蜂鸣器控制
    if(buzzerStr.Enable && !buzzerStr.Silent)
    {
//luy 叫够次数了
        if(buzzerStr.Times<=0)
        {
            BUZZER_OFF;
            buzzerStr.Enable = false;
            return;
        }
//luy 没叫够次数;叫完当前次序的时间
        else if(buzzerStr.Cut<=buzzerStr.Counter)
        {
            BUZZER_REV;
            buzzerStr.Times--;
            buzzerStr.Counter = 0;
        }
    }
    else
        BUZZER_OFF;

    //LED控制
    if(gpioStr.CounterLed > 100)        //100ms
    {
        LED_REV;
        gpioStr.CounterLed = 0;
    }
}


/**
* @brief        蜂鸣器使能
* @param        buzzer：蜂鸣器工作模式
* @ref
* @author       Leo
* @note
**/
void GPIO_BuzzerEnable(BuzzerEnum buzzer)
{
    switch(buzzer)
    {
        case BuzzerOk:
            buzzerStr.Cut = 70;         //70ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 4;
            break;

        case BuzzerWarnning:
            buzzerStr.Cut = 100;        //100ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 10;
            break;

        case BuzzerSysStart:
            buzzerStr.Cut = 60;         //60ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 6;
            break;

        case BuzzerDing:
            buzzerStr.Cut = 30;         //30ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 2;
            break;

        case BuzzerFinish:
            buzzerStr.Cut = 200;        //200ms
            buzzerStr.Enable = true;
            buzzerStr.Times = 6;
            break;
    }

    buzzerStr.Counter = 0;
}


