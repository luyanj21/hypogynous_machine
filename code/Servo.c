#include "servo.h"
//���
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

ServoStruct servoStr;


/**
* @brief        ������Ƴ�ʼ��
* @param
* @ref
* @author       Leo
* @note
**/
void SERVO_Init(void)
{
    pwm_init(SERVO_MOTOR_PWM,100, servoStr.thresholdMiddle);  //100hz��Ӧ��ֵ1500
}


/**
* @brief        ������PWM����
* @param        pwm��-20000~20000
* @ref
* @author       Leo
* @note
**/
uint32 PWMVALUE=0;
void SERVO_SetPwmValue(signed int pwm)
{
    pwm = 3000 - pwm;  //�����

    if(pwm < SERVO_PWM_MIN)
        pwm = SERVO_PWM_MIN;
    else if(pwm > SERVO_PWM_MAX)
        pwm = SERVO_PWM_MAX;

    pwm_set_duty(SERVO_MOTOR_PWM,pwm);
    PWMVALUE=pwm;
}


/**
 * �������
* @brief        ������PWM���ã�������
* @param        pwm��500~2500
* @ref
* @author       Leo
* @note
**/
uint16_t pwm_Servo = 0;
void SERVO_SetPwmValueCorrect(signed int pwm)
{
    pwm = 3000 - pwm;  //�����

    pwm -= servoStr.thresholdMiddle-SERVO_PWM_MIDDLE; //��ֵ����

    uint16_t pwmMax = 3000 - servoStr.thresholdLeft;
    uint16_t pwmMin = 3000 - servoStr.thresholdRight;
    if(pwm < pwmMin)
        pwm = pwmMin;
    else if(pwm > pwmMax)
        pwm = pwmMax;

    pwm_Servo =(uint16_t)pwm;
    pwm_set_duty(SERVO_MOTOR_PWM,pwm);
    PWMVALUE=pwm;
}

/**
* @brief       ����Ƕȿ���
* @param
* @ref
* @author
* @note
**/
uint16_t ServoPwm = 1500;
void SERVO_AngleControl(float angle)
{
    uint16_t pwm = 1500;
    angle = -angle;
    if(angle > SERVO_ANGLE_MAX)
        angle = SERVO_ANGLE_MAX;
    else if(angle < -SERVO_ANGLE_MAX)
        angle = -SERVO_ANGLE_MAX;

    if(angle >= 0)  //��ת
        pwm = (float)angle/SERVO_ANGLE_MAX * (SERVO_PWM_MAX_R-servoStr.thresholdMiddle) + servoStr.thresholdMiddle;     //���ԽǶȼ���
    else if(angle < 0)      //��ת
        pwm = (float)angle/SERVO_ANGLE_MAX * (servoStr.thresholdMiddle - SERVO_PWM_MAX_L) + servoStr.thresholdMiddle;       //���ԽǶȼ���

    ServoPwm = pwm;
    SERVO_SetPwmValue(pwm);
}

