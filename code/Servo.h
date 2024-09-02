#ifndef __SERVO_H__
#define __SERVO_H__

#include "main.h"
//#include "zf_common_headfile.h"


#define  SERVO_PWM_MAX                  2500//7500//2500                        //����������PWM��180��
#define  SERVO_PWM_MIN                  500//1500//500                         //���������СPWM��0��
#define  SERVO_PWM_MAX_L                1000//3450//1150                        //�������ת�����ֵPWM
#define  SERVO_PWM_MAX_R                1850//5550//1850                        //�������ת�����ֵPWM
#define  SERVO_PWM_MIDDLE               1500//4500//1500                          //�����ֵPWM

#define  SERVO_ANGLE_MAX                38.0f                       //���

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // ���������϶����Ӧ����
//#define SERVO_MOTOR_FREQ            (72 )       //ָ���Ƕ��Ƶ�ʣ���Դ�ϸ��õ�50�������õ�72                                // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300

extern uint32 PWMVALUE;


/**
* @brief    ������
**/
typedef struct
{
    uint16_t thresholdMiddle;                   //�����ֵPWM
    uint16_t thresholdLeft;                     //�������ת�����ֵPWM;    �ڶ��USB�������б�д��
    uint16_t thresholdRight;                    //�������ת�����ֵPWM
}ServoStruct;

extern ServoStruct servoStr;

void SERVO_Init(void);
void SERVO_SetPwmValue(signed int pwm);
void SERVO_SetPwmValueCorrect(signed int pwm);
void SERVO_AngleControl(float angle);
#endif

//===========================================  End Of File  ===========================================//


