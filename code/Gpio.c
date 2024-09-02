#include "Gpio.h"
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
//�����������ⲿ�ж�:ʹ��ͨ���� �˿�λ33.7 ERU_CH4_REQ8_P33_7
//�жϷ�����



GpioStruct gpioStr;
BuzzerStruct buzzerStr;

/**
* @brief        GPIO�����ʼ��
* @param
* @ref
* @author       Leo
* @note         ���裺������ x1  ״̬LED x1  �������� x1
**/
void GPIO_Initialize(void)
{
    /*-----------------------------------------LED-----------------------------------------*/
    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    /*-----------------------------------------����-----------------------------------------*/
    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
    /*-----------------------------------------������-----------------------------------------*/
    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);                        // ��ʼ�� BUZZER_PIN ��� Ĭ�ϵ͵�ƽ �������ģʽ
    //�����ش���
    exti_init(EXTI_PORT1, EXTI_TRIGGER_BOTH);             // ʹ�õ��ⲿ�ж���������

    //BuzzerInit
    buzzerStr.Counter = 0;
    buzzerStr.Cut = 0;
    //luyanj:������ʹ�ܸ�Ϊ��
    buzzerStr.Enable = true;
    //luyanj:��Ĭʹ�ܹ�
    buzzerStr.Silent = false;
    buzzerStr.Times = 0;
}


//----------------------------------------------[UNIT-���������������ж�]----------------------------------------------------------

/**
* @brief        ����A�ж���Ӧ����
* @param
* @ref
* @author       Leo
* @note
**/
//�ж����ȼ�����ռ�������ȼ���Ϊ1
 IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    if(exti_flag_get(EXTI_PORT1)==1)
    {
        if(!gpio_get_level(KEY1))    //��������
        {
            //luyanj:
//            ips114_show_string( 0 , 16*2,   "key_pressing");                            // ��ʾ�ַ���
            gpioStr.KeyPress = true;
            GPIO_BuzzerEnable(BuzzerDing);
        }
        else    //��������
        {
            gpioStr.KeyPress = false;
            //luyanj:
//            ips114_show_string( 0 , 16*2,   "key_released");                            // ��ʾ�ַ���
        }
    }
    exti_flag_clear(EXTI_PORT1);
}




// IFX_INTERRUPT(exti_ch1_ch5_isr, 0, EXTI_CH1_CH5_INT_PRIO)
// {
//     interrupt_global_enable(0);                     // �����ж�Ƕ��
//     if(exti_flag_get(ERU_CH1_REQ10_P14_3))          // ͨ��1�ж�
//     {
//         exti_flag_clear(ERU_CH1_REQ10_P14_3);
//     }
//
//     if(exti_flag_get(EXTI_PORT1))           // ͨ��5�ж�
//     {
//         exti_flag_clear(EXTI_PORT1);
//         if(!gpio_get_level(KEY1))    //��������
//         {
//             //luyanj:
//             ips114_show_string( 0 , 16*2,   "key_pressing");                            // ��ʾ�ַ���
//             gpioStr.KeyPress = true;
////             GPIO_BuzzerEnable(BuzzerDing);
//         }
//         else    //��������
//         {
//             gpioStr.KeyPress = false;
//             //luyanj:
//             ips114_show_string( 0 , 16*2,   "key_released");                            // ��ʾ�ַ���
//         }

//     }
// }

//IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
//{
//    interrupt_global_enable(0);                     // �����ж�Ƕ��
//    if(exti_flag_get(ERU_CH3_REQ6_P02_0))           // ͨ��3�ж�
//    {
//        exti_flag_clear(ERU_CH3_REQ6_P02_0);
//        camera_vsync_handler();                     // ����ͷ�����ɼ�ͳһ�ص�����
//    }
//    if(exti_flag_get(EXTI_PORT1))          // ͨ��7�ж�
//    {
//        exti_flag_clear(EXTI_PORT1);
//               if(!gpio_get_level(KEY1))    //��������
//               {
//                   //luyanj:
//                   ips114_show_string( 0 , 16*2,   "key_pressing");                            // ��ʾ�ַ���
//                   gpioStr.KeyPress = true;
//      //             GPIO_BuzzerEnable(BuzzerDing);
//               }
//               else    //��������
//               {
//                   gpioStr.KeyPress = false;
//                   //luyanj:
//                   ips114_show_string( 0 , 16*2,   "key_released");                            // ��ʾ�ַ���
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
* @brief        GPIO�߳̿�����
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Timer(void)
{
    //����������
    if(buzzerStr.Enable)
    {
        buzzerStr.Counter++;
//luy �������˳��Ƿ�ֹ�����
        if(buzzerStr.Cut<buzzerStr.Counter)
            buzzerStr.Counter = buzzerStr.Cut;
    }

    //LED��˸
    gpioStr.CounterLed++;
}



/**
* @brief        GPIO�߼�������
* @param
* @ref
* @author       Leo
* @note
**/
void GPIO_Handle(void)
{
    //����������
    if(buzzerStr.Enable && !buzzerStr.Silent)
    {
//luy �й�������
        if(buzzerStr.Times<=0)
        {
            BUZZER_OFF;
            buzzerStr.Enable = false;
            return;
        }
//luy û�й�����;���굱ǰ�����ʱ��
        else if(buzzerStr.Cut<=buzzerStr.Counter)
        {
            BUZZER_REV;
            buzzerStr.Times--;
            buzzerStr.Counter = 0;
        }
    }
    else
        BUZZER_OFF;

    //LED����
    if(gpioStr.CounterLed > 100)        //100ms
    {
        LED_REV;
        gpioStr.CounterLed = 0;
    }
}


/**
* @brief        ������ʹ��
* @param        buzzer������������ģʽ
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


