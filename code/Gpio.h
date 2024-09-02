#ifndef __GPIO_H__
#define __GPIO_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/

#include "main.h"
#include "stdbool.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

#define LED_ON          (gpio_set_level(LED1, led_state))
#define LED_OFF         (gpio_set_level(LED1, !led_state))
#define LED_REV         (gpio_toggle_level(LED1))

//更改为TC264
#define BUZZER_ON        (gpio_set_level(BUZZER_PIN, GPIO_HIGH))
#define BUZZER_OFF       (gpio_set_level(BUZZER_PIN, GPIO_LOW))
#define BUZZER_REV       (gpio_toggle_level(BUZZER_PIN))

/*--------------------------------------- tc264 引脚---------------------------------------*/
#define BUZZER_PIN              (P33_10)
#define LED1                    (P21_4)
#define KEY1                    (P20_9)
//设置eru通道及引脚
#define EXTI_PORT1              (ERU_CH7_REQ11_P20_9)


/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
/**
* @brief    蜂鸣器音效
**/
typedef enum
{
    BuzzerOk = 0,                       //确认提示音
    BuzzerWarnning,                     //报警提示音
    BuzzerSysStart,                     //开机提示音
    BuzzerDing,                         //叮=====(￣▽￣*)
    BuzzerFinish,                       //结束提示音
}BuzzerEnum;


/**
* @brief    按键和LED相关
**/
typedef struct
{
    bool KeyPress;                      //按键输入-B
    uint16_t CounterLed;                //LED闪烁计数器
}GpioStruct;


/**
* @brief    蜂鸣器相关
**/
typedef struct
{
    bool Enable;                        //使能标志
    uint16_t Times;                     //鸣叫次数
    uint16_t Counter;                   //计数器
    uint16_t Cut;                       //间隔时间
    bool Silent;                        //是否禁用蜂鸣器
}BuzzerStruct;


extern GpioStruct gpioStr;
extern BuzzerStruct buzzerStr;

void GPIO_Initialize(void);
void GPIO_Timer(void);
void GPIO_Handle(void);
void GPIO_BuzzerEnable(BuzzerEnum buzzer);

//中断
void exti_ch3_ch7_isr();
#endif

//===========================================  End Of File  ===========================================//

