 #include "encoder.h"

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
* @brief        ��������ʼ��
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


//��ע�͵ĵط��������ö�ʱ���ĵط����Ҳ�ȷ���÷���Ȼ��͸����˿�Դ�ϸ�Ĵ���





/**
* @brief        ������ת�ٲ���
* @param
* @ref
* @author       Leo
* @note
**/
void ENCODER_RevSample(void)
{
//luy ΪʲôҪ�Ӹ��ţ�

    motorStr.EncoderValue = - encoder_get_count(ENCODER_1);  //ɾȥ������
    encoder_clear_count(ENCODER_1);  //����������

//luy ��Ϊֻ���������ߵ�ʱ��Ż��нϴ���ٶ�����ֻ�ô������磨���룩
    if(motorStr.EncoderValue > 32767)
        motorStr.EncoderValue = motorStr.EncoderValue -  65536;

    //PID��������������
    pidStr.vi_FeedBack =(float) motorStr.EncoderValue;

    /*
     * 1.���ٱ�            ���Ȧ����������תȦ��֮��ı���
     * 2.����ֱ��
     * 3.���������
     * 4.ÿȦ�ж�������
     * 3.�����תȦ��
     *
     * */

    //����ʵ���ٶ�    ---     m/s
    icarStr.SpeedFeedback = (float)(motorStr.EncoderValue * PI * motorStr.DiameterWheel)/ MOTOR_CONTROL_CYCLE / motorStr.EncoderLine /  motorStr.ReductionRatio; //  m/s

    //luy��¼����ٶ�
    if(icarStr.SpeedFeedback > 0 && icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = icarStr.SpeedFeedback;

    //luy���������
    else if(icarStr.SpeedFeedback < 0 && -icarStr.SpeedFeedback > icarStr.SpeedMaxRecords)
        icarStr.SpeedMaxRecords = -icarStr.SpeedFeedback;
}



//luy ���Ա���������Ҫ���ж�
//IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
//{
//    interrupt_global_enable(0);                     // �����ж�Ƕ��
//    pit_clear_flag(CCU60_CH0);
//
//    ENCODER_RevSample();
//}






