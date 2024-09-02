#include "motor.h"
//#include "main.h"

//电机初始化
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

MotorStruct motorStr;


/**
* @brief        电机控制初始化
* @param
* @ref
* @author       Leo
* @note
**/
void MOTOR_Init(void)
{
    gpio_init(DIR1, GPO, 1, GPO_PUSH_PULL);          //电机转动方向这里调节，改  GPIO_HIGH或者 GPIO_LOW                // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM1, 17000 , 0);                                                   // PWM 通道初始化频率 17KHz 占空比初始为 0


    //电机模型初始化
    motorStr.EncoderLine = 512.0f;                          //编码器线数=光栅数16*4
    motorStr.ReductionRatio = 2.7f;                         //电机减速比
    motorStr.EncoderValue = 0;
    motorStr.DiameterWheel = 0.064f;//68cm                  //轮子直径:m
    motorStr.CloseLoop = true;                              //默认闭环模式
}


/**
* @brief        电机输出PWM设置
* @param        pwm：-2000~2000
* @ref
* @author       Leo
* @note
**/
void MOTOR_SetPwmValue(signed int pwm)
{
    //复用pid导致的符号不兼容？
    //luy不知道为什么打的负号pid没有复用
    pwm = -pwm;

    if(pwm>=0)
    {
        //pin14电机方向io口
        //设定电机正反转
        gpio_set_level(DIR1, GPIO_LOW);                                    // DIR输出高电平
        //pwm限幅
        if(pwm>MOTOR_PWM_MAX)
            pwm =MOTOR_PWM_MAX;
        //pwm
        pwm_set_duty(PWM1, pwm);                 // 计算占空比
    }
    else if(pwm<0)
    {
        gpio_set_level(DIR1,GPIO_HIGH );                                     // DIR输出低电平
        if(pwm<MOTOR_PWM_MIN)
            pwm=MOTOR_PWM_MIN;

        pwm = -pwm;

        pwm_set_duty(PWM1,pwm);
    }

}


/**
* @brief        电机闭环速控
* @param        speed：速度m/s
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


    //luy 传入pid计算的目的设定值
    //                  设定速度  电机转动周期            轮子直径                     π   编码器
    pidStr.vi_Ref = (float)(speed*MOTOR_CONTROL_CYCLE / motorStr.DiameterWheel / PI * motorStr.EncoderLine  * motorStr.ReductionRatio);

    //传入了设定值，找到了实际值，输出pid算出来的输出值
    MOTOR_SetPwmValue(PID_MoveCalculate(&pidStr));
}


/**
* @brief        电机控制线程
* @param
* @ref          猜测：放在主循环里面  错啦，放timer里
* @author       Leo
* @note
**/
void MOTOR_Timer(void)
{
    //luy 计算timer中断触发次数 （一次1ms），也就是间隔10ms执行一次
    motorStr.Counter++;
    if(motorStr.Counter >= 5)                              //速控:10ms
    {


        //luy 写入  实际速度      （SpeedFeedback），
        //         PID速度反馈值 （vi_FeedBack）
        ENCODER_RevSample();                                //编码器采样



        if(icarStr.sprintEnable || usbStr.connected)        //通信连接或电机测试才开启闭环（保护+省电）
        {



            if(motorStr.CloseLoop)
            {
//luy   ①限幅     ②含PID速控
                //向这个函数传入设定值：speed；SpeedSet是描述的设定值
                MOTOR_ControlLoop(icarStr.SpeedSet);        //闭环速控
            }



            else//开环百分比控制
            {
                if(icarStr.SpeedSet > 100)
                    icarStr.SpeedSet = 100;
                else if(icarStr.SpeedSet < -100)
                    icarStr.SpeedSet = -100;
                signed int speedRate = MOTOR_PWM_MAX/100.f*icarStr.SpeedSet; //开环：百分比%

                MOTOR_SetPwmValue(speedRate);       //开环速控
            }
        }


        else
        {
            MOTOR_SetPwmValue(0);
        }

        motorStr.Counter = 0;
    }
}

