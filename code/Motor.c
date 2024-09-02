#include "motor.h"
//#include "main.h"

//�����ʼ��
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

MotorStruct motorStr;


/**
* @brief        ������Ƴ�ʼ��
* @param
* @ref
* @author       Leo
* @note
**/
void MOTOR_Init(void)
{
    gpio_init(DIR1, GPO, 1, GPO_PUSH_PULL);          //���ת������������ڣ���  GPIO_HIGH���� GPIO_LOW                // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM1, 17000 , 0);                                                   // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0


    //���ģ�ͳ�ʼ��
    motorStr.EncoderLine = 512.0f;                          //����������=��դ��16*4
    motorStr.ReductionRatio = 2.7f;                         //������ٱ�
    motorStr.EncoderValue = 0;
    motorStr.DiameterWheel = 0.064f;//68cm                  //����ֱ��:m
    motorStr.CloseLoop = true;                              //Ĭ�ϱջ�ģʽ
}


/**
* @brief        ������PWM����
* @param        pwm��-2000~2000
* @ref
* @author       Leo
* @note
**/
void MOTOR_SetPwmValue(signed int pwm)
{
    //����pid���µķ��Ų����ݣ�
    //luy��֪��Ϊʲô��ĸ���pidû�и���
    pwm = -pwm;

    if(pwm>=0)
    {
        //pin14�������io��
        //�趨�������ת
        gpio_set_level(DIR1, GPIO_LOW);                                    // DIR����ߵ�ƽ
        //pwm�޷�
        if(pwm>MOTOR_PWM_MAX)
            pwm =MOTOR_PWM_MAX;
        //pwm
        pwm_set_duty(PWM1, pwm);                 // ����ռ�ձ�
    }
    else if(pwm<0)
    {
        gpio_set_level(DIR1,GPIO_HIGH );                                     // DIR����͵�ƽ
        if(pwm<MOTOR_PWM_MIN)
            pwm=MOTOR_PWM_MIN;

        pwm = -pwm;

        pwm_set_duty(PWM1,pwm);
    }

}


/**
* @brief        ����ջ��ٿ�
* @param        speed���ٶ�m/s
* @ref
* @author       Leo
* @note
**/
void MOTOR_ControlLoop(float speed)
{
    if(speed > MOTOR_SPEED_MAX)
        speed = MOTOR_SPEED_MAX;
    else if(speed < -MOTOR_SPEED_MAX)
        speed = -MOTOR_SPEED_MAX;


    //luy ����pid�����Ŀ���趨ֵ
    //                  �趨�ٶ�  ���ת������            ����ֱ��                     ��   ������
    pidStr.vi_Ref = (float)(speed*MOTOR_CONTROL_CYCLE / motorStr.DiameterWheel / PI * motorStr.EncoderLine  * motorStr.ReductionRatio);

    //�������趨ֵ���ҵ���ʵ��ֵ�����pid����������ֵ
    MOTOR_SetPwmValue(PID_MoveCalculate(&pidStr));
}


/**
* @brief        ��������߳�
* @param
* @ref          �²⣺������ѭ������  ��������timer��
* @author       Leo
* @note
**/
void MOTOR_Timer(void)
{
    //luy ����timer�жϴ������� ��һ��1ms����Ҳ���Ǽ��10msִ��һ��
    motorStr.Counter++;
    if(motorStr.Counter >= 5)                              //�ٿ�:10ms
    {


        //luy д��  ʵ���ٶ�      ��SpeedFeedback����
        //         PID�ٶȷ���ֵ ��vi_FeedBack��
        ENCODER_RevSample();                                //����������



        if(icarStr.sprintEnable || usbStr.connected)        //ͨ�����ӻ������Բſ����ջ�������+ʡ�磩
        {



            if(motorStr.CloseLoop)
            {
//luy   ���޷�     �ں�PID�ٿ�
                //��������������趨ֵ��speed��SpeedSet���������趨ֵ
                MOTOR_ControlLoop(icarStr.SpeedSet);        //�ջ��ٿ�
            }



            else//�����ٷֱȿ���
            {
                if(icarStr.SpeedSet > 100)
                    icarStr.SpeedSet = 100;
                else if(icarStr.SpeedSet < -100)
                    icarStr.SpeedSet = -100;
                signed int speedRate = MOTOR_PWM_MAX/100.f*icarStr.SpeedSet; //�������ٷֱ�%

                MOTOR_SetPwmValue(speedRate);       //�����ٿ�
            }
        }


        else
        {
            MOTOR_SetPwmValue(0);
        }

        motorStr.Counter = 0;
    }
}

