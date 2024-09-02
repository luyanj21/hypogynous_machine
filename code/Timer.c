#include "Timer.h"
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


/**
* @brief        ��ʱ��TIM2��ʼ��
* @param        
* @ref          
* @author       Leo
* @note         
**/
void Timer_Init(void)
{
    pit_ms_init(PIT_NUM, 1);  // ��ʼ�� CCU6_0_CH0 Ϊ�����ж� 1ms ����
}


/**
* @brief        CCU61_CH0��ʱ�жϷ���
* @param        
* @ref          
* @author       Leo
* @note         
**/
//luy��Ϊ����������ʱ���Ǻ�����жϹҹ��ģ����Ա���Ҫ1msһ��
IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU61_CH0);

//1.
/*luy �Է�����һ��timer����е�ʱ������������־��cut<counter*/
    GPIO_Timer();           //GPIO�����߳�

//2.
//luy ����������
//luy ����ʹ��||��λ�����ӣ����ջ������ת����
    MOTOR_Timer();          //��������߳�


//3.
//    SOC_Timer();            //�����Ƽ���߳�

//4.
/*luy�ջ���̺Ͱ�����ʱ*/
    ICAR_Timer();           //���ܳ��ۺϴ����̼߳�����


//5.
/*luy ���߼��*/
    USB_Edgeboard_Timr();   //USBͨ���߳�
}












