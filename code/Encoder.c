 #include "encoder.h"

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

/**
* @brief        编码器初始化
* @param
* @ref
* @author       Leo
* @note
**/
int16 encoder_data_1 = 0;

void ENCODER_Init(void)
{
    encoder_quad_init(ENCODER_1,ENCODER_1_A,ENCODER_1_B);
    encoder_clear_count(ENCODER_1);
//    pit_ms_init(CCU60_CH0, 100);

}


//打注释的地方是赛曙用定时器的地方，我不确定用法，然后就改用了开源老哥的代码





/**
* @brief        编码器转速采样
* @param
* @ref
* @author       Leo
* @note
**/
void ENCODER_RevSample(void)
{
//luy 为什么要加负号？

    motorStr.EncoderValue = - encoder_get_count(ENCODER_1);  //删去符号了
    encoder_clear_count(ENCODER_1);  //计数器清零

//luy 因为只有在正向走的时候才会有较大的速度所以只用处理上溢（补码）
    if(motorStr.EncoderValue > 32767)
        motorStr.EncoderValue = motorStr.EncoderValue -  65536;

    //PID负反馈数据输入
    pidStr.vi_FeedBack =(float) motorStr.EncoderValue;

    /*
     * 1.减速比            电机圈数和轮子旋转圈数之间的比例
     * 2.轮子直径
     * 3.测得脉冲数
     * 4.每圈有多少脉冲
     * 3.电机旋转圈数
     *
     * */

    //计算实际速度    ---     m/s
    icarStr.SpeedFeedback = (float)(motorStr.EncoderValue * PI * motorStr.DiameterWheel)/ MOTOR_CONTROL_CYCLE / motorStr.EncoderLine /  motorStr.ReductionRatio; //  m/s

    //luy记录最高速度
    if(icarStr.SpeedFeedback > 0 && icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = icarStr.SpeedFeedback;

    //luy反向最高速
    else if(icarStr.SpeedFeedback < 0 && -icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = -icarStr.SpeedFeedback;
}



//luy 测试编码器，需要的中断
//IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
//{
//    interrupt_global_enable(0);                     // 开启中断嵌套
//    pit_clear_flag(CCU60_CH0);
//
//    ENCODER_RevSample();
//}






