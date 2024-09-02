#ifndef __GPIO_H__
#define __GPIO_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/

#include "main.h"
#include "stdbool.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

#define LED_ON          (gpio_set_level(LED1, led_state))
#define LED_OFF         (gpio_set_level(LED1, !led_state))
#define LED_REV         (gpio_toggle_level(LED1))

//����ΪTC264
#define BUZZER_ON        (gpio_set_level(BUZZER_PIN, GPIO_HIGH))
#define BUZZER_OFF       (gpio_set_level(BUZZER_PIN, GPIO_LOW))
#define BUZZER_REV       (gpio_toggle_level(BUZZER_PIN))

/*--------------------------------------- tc264 ����---------------------------------------*/
#define BUZZER_PIN              (P33_10)
#define LED1                    (P21_4)
#define KEY1                    (P20_9)
//����eruͨ��������
#define EXTI_PORT1              (ERU_CH7_REQ11_P20_9)


/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
/**
* @brief    ��������Ч
**/
typedef enum
{
    BuzzerOk = 0,                       //ȷ����ʾ��
    BuzzerWarnning,                     //������ʾ��
    BuzzerSysStart,                     //������ʾ��
    BuzzerDing,                         //��=====(������*)
    BuzzerFinish,                       //������ʾ��
}BuzzerEnum;


/**
* @brief    ������LED���
**/
typedef struct
{
    bool KeyPress;                      //��������-B
    uint16_t CounterLed;                //LED��˸������
}GpioStruct;


/**
* @brief    ���������
**/
typedef struct
{
    bool Enable;                        //ʹ�ܱ�־
    uint16_t Times;                     //���д���
    uint16_t Counter;                   //������
    uint16_t Cut;                       //���ʱ��
    bool Silent;                        //�Ƿ���÷�����
}BuzzerStruct;


extern GpioStruct gpioStr;
extern BuzzerStruct buzzerStr;

void GPIO_Initialize(void);
void GPIO_Timer(void);
void GPIO_Handle(void);
void GPIO_BuzzerEnable(BuzzerEnum buzzer);

//�ж�
void exti_ch3_ch7_isr();
#endif

//===========================================  End Of File  ===========================================//

