#include "Icar.h"


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


IcarStruct icarStr;

/**
* @Description  : ���ܳ�������ʼ��
* @params       :
* @Date         :
* @author       : Leo
* @notes        :
**/
void ICAR_Init(void)
{
//luy:������ѹ�ò���
    icarStr.Electricity = 0;                    //������Ϣ
    icarStr.Voltage = 0;                        //��ѹ


    icarStr.SpeedSet = 0.0f;                    //���Ŀ���ٶȣ�m/s
    icarStr.SpeedFeedback = 0.0f;               //���ģ��ʵ���ٶȣ�m/s

    icarStr.SpeedMaxRecords = 0.0f;
    icarStr.ServoPwmSet = servoStr.thresholdMiddle;

    //���ܳ��Լ�flase
    icarStr.selfcheckEnable=false;
    //�ջ����ʹ�ܣ����Ե����
    icarStr.sprintEnable=false;
}



/**
* @brief        ���ܳ��ۺϴ����̼߳�����
* @param
* @ref
* @author       Leo
* @note
**/
void ICAR_Timer(void)
{
    //luy��ʱ
    //����
    if(gpioStr.KeyPress)
    {
        icarStr.counterKeyA++;
    }
    else
    {
        if(icarStr.counterKeyA > 100)//100ms
            icarStr.keyPressed = true;
    }

//    //�ջ����
//    if(icarStr.sprintEnable)
//    {
//        icarStr.counterSprint++;
//        if(icarStr.counterSprint == 2000)//�ȴ�����ʱ��2s
//        {
////            RGB_SetAllColor(RGB_COLOR_GREEN);
//            icarStr.SpeedSet = 2.0f;    //���Ŀ���ٶȣ�m/s
//        }
//    }
//    else
//    {
//        icarStr.counterSprint = 0;
//    }

// **************************** �Լ� ****************************
    //�Լ��ʱ
//    if(icarStr.selfcheckEnable)
//    {
//        icarStr.counterSelfcheck++;
//        icarStr.counterModuleCheck++;
//        switch((uint8_t)icarStr.selfcheckStep)
//        {
//            case Selfcheck_None:            //��ʼ����
//                if(icarStr.counterSelfcheck > 500)//500ms
//                {
//                    icarStr.selfcheckStep = Selfcheck_MotorA;
//                    icarStr.counterSelfcheck = 0;
//                }
//                break;
//            case Selfcheck_MotorA:          //�������A
//                if(icarStr.counterSelfcheck > 2000)//2s
//                {
//                    icarStr.selfcheckStep = Selfcheck_MotorB;
//                    icarStr.timesSendStep = 0;
//                    icarStr.counterSelfcheck = 0;
//                    icarStr.counterModuleCheck = 0;
//                }
//                break;
//            case Selfcheck_MotorB:          //�������
//                if(icarStr.counterSelfcheck > 3000)//3s
//                {
//                    icarStr.counterSelfcheck = 3000;
//                }
//                break;
//            case Selfcheck_MotorC:          //�������
//                if(icarStr.counterSelfcheck > 2000)//2s
//                {
//                    icarStr.selfcheckStep = Selfcheck_MotorD;
//                    icarStr.timesSendStep = 0;
//                    icarStr.counterSelfcheck = 0;
//                    icarStr.counterModuleCheck = 0;
//                }
//                break;
//            case Selfcheck_MotorD:          //�������
//                if(icarStr.counterSelfcheck > 3000)//3s
//                {
//                    icarStr.counterSelfcheck = 3000;
//                }
//                break;
//            case Selfcheck_MotorE:          //�������
//                if(icarStr.counterSelfcheck > 2000)//2s
//                {
//                    icarStr.selfcheckStep = Selfcheck_MotorF;
//                    icarStr.counterSelfcheck = 0;
//                    icarStr.counterModuleCheck = 0;
//                }
//                break;
//            case Selfcheck_MotorF:          //�������
//                if(icarStr.counterSelfcheck > 2000)//2s
//                {
//                    icarStr.counterSelfcheck = 2000;
//                }
//                break;
//            case Selfcheck_MotorG:          //�������
//                if(icarStr.counterSelfcheck > 2000)//2s
//                {
//                    icarStr.selfcheckStep = Selfcheck_MotorH;
//                    icarStr.counterSelfcheck = 0;
//                    icarStr.counterModuleCheck = 0;
//                }
//                break;
//            case Selfcheck_MotorH:          //�������
//                if(icarStr.counterSelfcheck > 2000)//2s
//                {
//                    icarStr.counterSelfcheck = 2000;
//                }
//                break;
//            case Selfcheck_ServoA:          //�������
//                if(icarStr.counterSelfcheck > 5000)//2s
//                {
//                    icarStr.counterSelfcheck = 5000;
//                }
//                break;
//            case Selfcheck_Com:             //ͨ�Ų���
//                if(icarStr.counterSelfcheck > 1000)//1s
//                {
//                    icarStr.selfcheckStep = Selfcheck_Buzzer;
//                    icarStr.counterSelfcheck = 0;
//                    icarStr.counterModuleCheck = 0;
//                }
//                break;
//            case Selfcheck_Buzzer:          //����������
//                if(icarStr.counterSelfcheck > 5000)//2s
//                {
//                    icarStr.counterSelfcheck = 5000;
//                }
//                break;
//            case Selfcheck_RgbLed:          //��Ч����
//                if(icarStr.counterSelfcheck > 4000)//2s
//                {
//                    icarStr.counterSelfcheck = 4000;
//                }
//                break;
//            case Selfcheck_Key:             //��������
//                if(icarStr.counterSelfcheck > 5000)//2s
//                {
//                    icarStr.counterSelfcheck = 5000;
//                }
//                break;
//
//        }
//    }
// **************************** sasu ****************************

}


/**
* @Description  : ���ܳ��ۺϴ�����
* @params       :
* @Date         :
* @author       : Leo
* @notes        :
**/
void ICAR_Handle(void)
{



// **************************** ִ�� ****************************
    //������λ����
    if(icarStr.keyPressed)
    {
        if(icarStr.counterKeyA > 100)   //100ms��Ч
            USB_Edgeboard_TransmitKey(icarStr.counterKeyA); //���Ͱ���ʱ��
// **************************** ִ�� ****************************
        //�ջ����
//        if(icarStr.counterKeyA > 2000 && !icarStr.selfcheckEnable)//����2s���ջ����
//        {
//            icarStr.counterSprint = 0;
//
//            GPIO_BuzzerEnable(BuzzerOk);
//        }
// **************************** ִ�� ****************************
        icarStr.counterKeyA = 0;
        icarStr.keyPressed = false;
// **************************** ִ�� ****************************

//        if(icarStr.selfcheckEnable)//�Լ���԰����û�
//        {
//            icarStr.errorCode &= ~(1<<8);
//        }
    }



//    //�ջ���̣����Ե����
//    if(icarStr.sprintEnable)
//    {
//        if(icarStr.counterSprint > 3000)//���ʱ�䣺3s
//        {
//            GPIO_BuzzerEnable(BuzzerFinish);
////            RGB_SetAllColor(RGB_COLOR_RED);
//            icarStr.SpeedSet = 0.f;    //���Ŀ���ٶȣ�m/s
//            icarStr.sprintEnable = false;
//        }
//    }

    //���ܳ��Լ�
//    if(icarStr.selfcheckEnable)
//    {
//        ICAR_Selfcheck();
//    }
}


/**
* @brief        ���ư�����
* @param
* @ref
* @author       Leo
* @note
**/
void ICAR_Reboot(void)
{
//    __disable_fault_irq();
//    NVIC_SystemReset();
}







//----------------------------------------------[UNIT-���������Լ����]----------------------------------------------------------
/**
* @brief        ���ܳ��Լ�
* @param
* @ref
* @author       Leo
* @note
**/
void ICAR_Selfcheck(void)
{
    static bool errorMotor;
    static bool errorPID;
    switch(icarStr.selfcheckStep)
    {
        case Selfcheck_None:            //��ʼ����
            motorStr.CloseLoop = false;
            icarStr.timesSendStep = 0;
            break;
        case Selfcheck_MotorA:          //�����ת����
            motorStr.CloseLoop = false;
            icarStr.SpeedSet = 25.0f;   //25%
            icarStr.errorCode |= (1<<1);//2
            icarStr.errorCode |= (1<<2);//4
            errorMotor = true;
            break;
        case Selfcheck_MotorB:          //�����ת����
            if(ICAR_SpeedSample()>0.5)
            {
                icarStr.errorCode &= ~(1<<1);
                icarStr.errorCode &= ~(1<<2);
                icarStr.selfcheckStep = Selfcheck_MotorC;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorB);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                errorMotor = false;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 3000)//��ʱ
            {
                if(icarStr.timesSendStep < 5)
                {
                    USB_Edgeboard_Selfcheck(Selfcheck_MotorB);
                    icarStr.timesSendStep++;
                }
                icarStr.counterSelfcheck = 0;
            }
            break;
        case Selfcheck_MotorC:          //�����ת����
            motorStr.CloseLoop = false;
            icarStr.SpeedSet = -25.0f;
            break;
        case Selfcheck_MotorD:          //�����ת����
            if(ICAR_SpeedSample()<-0.5)
            {
                if(!errorMotor)
                    icarStr.errorCode &= ~(1<<1);

                icarStr.selfcheckStep = Selfcheck_MotorE;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorD);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 3000)//��ʱ
            {
                icarStr.errorCode |= (1<<1);

                if(icarStr.timesSendStep < 5)
                {
                    USB_Edgeboard_Selfcheck(Selfcheck_MotorD);
                    icarStr.timesSendStep++;
                }
            }
            break;
        case Selfcheck_MotorE:          //����ջ���������
            icarStr.SpeedSet = 1.0f;
            motorStr.CloseLoop = true;
            icarStr.errorCode |= (1<<3);
            errorPID = true;
            break;
        case Selfcheck_MotorF:          //����ջ���������
            if( CMATH_AbsFloat(ICAR_SpeedSample()-icarStr.SpeedSet) < 0.3)
            {
                icarStr.errorCode &= ~(1<<3);
                icarStr.selfcheckStep = Selfcheck_MotorG;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorF);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                errorPID = false;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 2000)//��ʱ
            {
                USB_Edgeboard_Selfcheck(Selfcheck_MotorF);
                icarStr.selfcheckStep = Selfcheck_ServoA;
                icarStr.timesSendStep = 0;
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
            }
            break;
        case Selfcheck_MotorG:          //����ջ���ת����
            motorStr.CloseLoop = true;
            icarStr.SpeedSet = -1.0f;
            icarStr.errorCode |= (1<<3);
            icarStr.timesSendStep = 0;
            break;
        case Selfcheck_MotorH:          //����ջ���ת����
            if( CMATH_AbsFloat(ICAR_SpeedSample()-icarStr.SpeedSet) < 0.3 && !errorPID)
            {
                icarStr.errorCode &= ~(1<<3);
                icarStr.selfcheckStep = Selfcheck_ServoA;
                icarStr.timesSendStep = 0;
                USB_Edgeboard_Selfcheck(Selfcheck_MotorH);
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
                GPIO_BuzzerEnable(BuzzerDing);
            }
            else if(icarStr.counterSelfcheck >= 2000)//��ʱ
            {
                USB_Edgeboard_Selfcheck(Selfcheck_MotorH);
                icarStr.selfcheckStep = Selfcheck_ServoA;
                icarStr.timesSendStep = 0;
                icarStr.counterSelfcheck = 0;
                icarStr.counterModuleCheck = 0;
            }
            break;

        case Selfcheck_ServoA:          //�������
            motorStr.CloseLoop = false;
            icarStr.SpeedSet = 0.0f;
            icarStr.timesSendStep = 0;
            ICAR_ServoCheck();
            if(icarStr.counterSelfcheck >= 2000 && icarStr.timesSendStep < 5)//��ʱ
            {
                USB_Edgeboard_Selfcheck(Selfcheck_ServoA);
                icarStr.timesSendStep++;
            }
            break;

        case Selfcheck_Com:             //ͨ�Ų���
            icarStr.ServoPwmSet = servoStr.thresholdMiddle;
            SERVO_AngleControl(icarStr.ServoPwmSet);
            icarStr.selfcheckStep = Selfcheck_Buzzer;
            break;

        case Selfcheck_Buzzer:          //����������
            ICAR_BuzzerCheck();
            if(icarStr.counterSelfcheck >= 1000 && icarStr.timesSendStep < 5)//��ʱ
            {
                USB_Edgeboard_Selfcheck(Selfcheck_Buzzer);
                icarStr.timesSendStep++;
            }
            break;
        case Selfcheck_RgbLed:          //��Ч����
            ICAR_RgbCheck();
            if(icarStr.counterSelfcheck >= 1000 && icarStr.timesSendStep < 5)//��ʱ
            {
                USB_Edgeboard_Selfcheck(Selfcheck_RgbLed);
                icarStr.timesSendStep++;
            }
            break;
        case Selfcheck_Key:             //��������
            if(icarStr.counterSelfcheck >= 1000 && icarStr.timesSendStep < 5)//��ʱ
            {
                USB_Edgeboard_Selfcheck(Selfcheck_Key);
                icarStr.timesSendStep++;
            }
            break;
        case Selfcheck_Finish:          //�������
            GPIO_BuzzerEnable(BuzzerFinish);
            icarStr.selfcheckEnable = false;
            ICAR_Reboot();//����
            break;
    }

}

/**
* @brief        �Լ����̿���
* @param
* @ref
* @author       Leo
* @note
**/
uint8_t resStep = 0;
void ICAR_SelfcheckControl(uint8_t step)
{
    resStep = step;
    if(step == (uint8_t)Selfcheck_None)
    {
//        RGB_SetAllColor(RGB_COLOR_GREEN);   //������ɫ�ƹ�
        GPIO_BuzzerEnable(BuzzerSysStart);
        icarStr.selfcheckEnable = true;
        icarStr.counterSelfcheck = 0;
        icarStr.selfcheckStep = Selfcheck_None;
        icarStr.errorCode |= (1<<8);
    }
    else
    {
        icarStr.counterSelfcheck = 0;
        icarStr.counterModuleCheck = 0;
        icarStr.selfcheckStep = resStep;
        icarStr.timesSendStep = 0;
//        GPIO_BuzzerEnable(BuzzerOk);
    }
}

/**
* @brief        �ջ��ٶȲ���
* @param
* @ref
* @author       Leo
* @note
**/
float buffSample[5] = {0};
float ICAR_SpeedSample(void)
{
    if(icarStr.counterModuleCheck>100)//100ms
    {
        switch(icarStr.speedSampleStep)
        {
            case 0:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 1:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 2:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 3:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep++;
                break;
            case 4:
                buffSample[icarStr.speedSampleStep] = icarStr.SpeedFeedback;
                icarStr.speedSampleStep = 0;
                ICAR_SortFloat(buffSample,0,5);
                return buffSample[2];
                break;
        }
        icarStr.counterModuleCheck = 0;
    }

    return 0;
}

/**
* @brief        �������
* @param
* @ref
* @author       Leo
* @note
**/
bool ICAR_ServoCheck(void)
{
    if(icarStr.counterModuleCheck < 1)
        icarStr.ServoPwmSet = servoStr.thresholdLeft;

    if(icarStr.counterModuleCheck>100)//100ms
    {
        icarStr.ServoPwmSet +=30;
        if(icarStr.ServoPwmSet >= servoStr.thresholdRight)
        {
            icarStr.ServoPwmSet = servoStr.thresholdLeft;
            return true;
        }
        SERVO_SetPwmValueCorrect(icarStr.ServoPwmSet);
        icarStr.counterModuleCheck = 1;
    }

    return false;
}

/**
* @brief        ����������
* @param
* @ref
* @author       Leo
* @note
**/
uint8_t indexBuzzerCheck = 0;
bool ICAR_BuzzerCheck(void)
{
    if(icarStr.counterModuleCheck>800)
    {
        indexBuzzerCheck++;
        switch(indexBuzzerCheck)
        {
            case 1:
                GPIO_BuzzerEnable(BuzzerDing);
                break;
            case 2:
//                GPIO_BuzzerEnable(BuzzerOk);
                break;
            case 3:
                GPIO_BuzzerEnable(BuzzerSysStart);
                break;
            case 4:
                GPIO_BuzzerEnable(BuzzerWarnning);
                break;
            case 5:
                GPIO_BuzzerEnable(BuzzerFinish);
                indexBuzzerCheck = 0;
                return true;
        }
        icarStr.counterModuleCheck = 0;
    }

    return false;
}

/**
* @brief        RGB�Ʋ���
* @param
* @ref
* @author       Leo
* @note
**/
uint8_t indexRgbCheck = 0;
void ICAR_RgbCheck(void)
{
    if(icarStr.counterModuleCheck>300)
    {
        indexRgbCheck++;
        switch(indexRgbCheck)
        {
            case 1:
//                RGB_SetAllColor(RGB_COLOR_RED);
            break;
            case 2:
//                RGB_SetAllColor(RGB_COLOR_GREEN);
            break;
            case 3:
//                RGB_SetAllColor(RGB_COLOR_BLUE);��
                indexRgbCheck = 0;
            break;
        }
        icarStr.counterModuleCheck = 0;
    }
}
/**
* @brief        ����������
* @param        buff:���ݴ洢���飬start����ʼλ��len�����򳤶�
* @ref
* @author       Leo
* @note
**/
void ICAR_SortFloat(float *buff, uint8_t start, uint8_t len)
{
    float n = 0;
    for(uint8_t i = start; i < start+len; i++)
    {
        for(uint8_t j = i+1; j<start+len; j++)
        {
            if(buff[i] > buff[j])
            {
                n = buff[i];
                buff[i] = buff[j];
                buff[j] = n;
            }
        }
    }
}


/**
* @brief        ����������ֵ����
* @param        data��Ҫ�����ֵ�ĵ����ȸ�����
* @ref
* @author       Leo
* @note
**/
float CMATH_AbsFloat(float data)
{
    if(data < 0)
        return -data;
    else
        return data;
}

//------------------------------------------------[END]-------------------------------------------------------------




