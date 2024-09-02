#ifndef __SERVO_H__
#define __SERVO_H__

#include "main.h"
//#include "zf_common_headfile.h"


#define  SERVO_PWM_MAX                  2500//7500//2500                        //舵机方向最大PWM：180°
#define  SERVO_PWM_MIN                  500//1500//500                         //舵机方向最小PWM：0°
#define  SERVO_PWM_MAX_L                1000//3450//1150                        //舵机左向转角最大值PWM
#define  SERVO_PWM_MAX_R                1850//5550//1850                        //舵机右向转角最大值PWM
#define  SERVO_PWM_MIDDLE               1500//4500//1500                          //舵机中值PWM

#define  SERVO_ANGLE_MAX                38.0f                       //舵机

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // 定义主板上舵机对应引脚
//#define SERVO_MOTOR_FREQ            (72 )       //指的是舵机频率，开源老哥用的50，赛曙用的72                                // 定义主板上舵机频率  请务必注意范围 50-300

extern uint32 PWMVALUE;


/**
* @brief    舵机相关
**/
typedef struct
{
    uint16_t thresholdMiddle;                   //舵机中值PWM
    uint16_t thresholdLeft;                     //舵机左向转角最大值PWM;    在舵机USB处理函数中被写入
    uint16_t thresholdRight;                    //舵机右向转角最大值PWM
}ServoStruct;

extern ServoStruct servoStr;

void SERVO_Init(void);
void SERVO_SetPwmValue(signed int pwm);
void SERVO_SetPwmValueCorrect(signed int pwm);
void SERVO_AngleControl(float angle);
#endif

//===========================================  End Of File  ===========================================//


