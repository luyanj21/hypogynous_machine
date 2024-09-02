#include "Pid.h"

//pid参数

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

PIDStruct pidStr;


/**
* @brief        PID参数初始化
* @param
可能舵机使用
电机使用：让电机达到我们的目标转速，设定值：？实际值编码器传入
* @ref
* @author       Leo
* @note
**/
void PID_Init(void)
{
    //传入的设定值
    pidStr.vi_Ref = 0 ;             //速度PID，速度设定值
    //a不知道哪来的：是编码器
    pidStr.vi_FeedBack = 0 ;        //速度PID，速度反馈值

    pidStr.vi_PreError = 0 ;        //速度PID，速度误差,vi_Ref - vi_FeedBack
    pidStr.vi_PreDerror = 0 ;       //速度PID，前一次，速度误差之差，d_error-PreDerror;
    pidStr.v_Kp = PID_VKP;          //比例系数，Kp = Kp
    pidStr.v_Ki = PID_VKI;          //积分系数，Ki = Kp * ( T / Ti )
    pidStr.v_Kd = PID_VKD;          //微分系数，Kd = KP * Td * T

    pidStr.vl_PreU = 0;             //PID输出值
}


/**
* @brief        PID速控模型
* @param        pid求值
* @ref
* @author       Leo
* @note
**/
signed int PID_MoveCalculate(PIDStruct *pp)
{
    float  error,d_error,dd_error;

    error = pp->vi_Ref - pp->vi_FeedBack;
    d_error = error - pp->vi_PreError;
    dd_error = d_error - pp->vi_PreDerror;

    pp->vi_PreError = error;
    pp->vi_PreDerror = d_error;

    if( ( error < VV_DEADLINE ) && ( error > -VV_DEADLINE ) )
    {
        ;
    }
    else
    {
//PID积分包和，必要时候启用，消抖
//              I_error = pp -> v_Ki * error;
//              if(I_error >= (VV_MAX/5))
//              {
//              I_error = VV_MAX/5;
//              }
//              else if(I_error <= (VV_MIN/5))
//              {
//              I_error = VV_MIN/5;
//              }
//        pp->vl_PreU += (pp -> v_Kp * d_error + I_error + pp->v_Kd*dd_error)/3;
                  pp->vl_PreU += (pp -> v_Kp * d_error + pp -> v_Ki * error + pp->v_Kd*dd_error);
    }
//        pp->vl_PreU = pp->vl_PreU;  //开源老哥把这行注释掉了，赛曙没有注释掉
    if( pp->vl_PreU >= MOTOR_PWM_MAX )
    {
        pp->vl_PreU = MOTOR_PWM_MAX;
    }
    else if( pp->vl_PreU <= MOTOR_PWM_MIN )
    {
        pp->vl_PreU = MOTOR_PWM_MIN;
    }

    return (signed int) (pp->vl_PreU);
}





