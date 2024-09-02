#include "servo.h"
//舵机
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

ServoStruct servoStr;


/**
* @brief        舵机控制初始化
* @param
* @ref
* @author       Leo
* @note
**/
void SERVO_Init(void)
{
    pwm_init(SERVO_MOTOR_PWM,100, servoStr.thresholdMiddle);  //100hz对应中值1500
}


/**
* @brief        舵机输出PWM设置
* @param        pwm：-20000~20000
* @ref
* @author       Leo
* @note
**/
uint32 PWMVALUE=0;
void SERVO_SetPwmValue(signed int pwm)
{
    pwm = 3000 - pwm;  //左→右

    if(pwm < SERVO_PWM_MIN)
        pwm = SERVO_PWM_MIN;
    else if(pwm > SERVO_PWM_MAX)
        pwm = SERVO_PWM_MAX;

    pwm_set_duty(SERVO_MOTOR_PWM,pwm);
    PWMVALUE=pwm;
}


/**
 * 舵机矫正
* @brief        舵机输出PWM设置（矫正后）
* @param        pwm：500~2500
* @ref
* @author       Leo
* @note
**/
uint16_t pwm_Servo = 0;
void SERVO_SetPwmValueCorrect(signed int pwm)
{
    pwm = 3000 - pwm;  //左→右

    pwm -= servoStr.thresholdMiddle-SERVO_PWM_MIDDLE; //中值补偿

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
* @brief       舵机角度控制
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

    if(angle >= 0)  //右转
        pwm = (float)angle/SERVO_ANGLE_MAX * (SERVO_PWM_MAX_R-servoStr.thresholdMiddle) + servoStr.thresholdMiddle;     //绝对角度计算
    else if(angle < 0)      //左转
        pwm = (float)angle/SERVO_ANGLE_MAX * (servoStr.thresholdMiddle - SERVO_PWM_MAX_L) + servoStr.thresholdMiddle;       //绝对角度计算

    ServoPwm = pwm;
    SERVO_SetPwmValue(pwm);
}

