#include "Usb.h"
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

UsbStruct usbStr;
uint8 rec_buff[USART_REC_LEN];
uint16 USB_RX_STA = 0;  //����״̬��־λ
/**
* @brief        USB/UART��ʼ��
* @param
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Init(void)
{
    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);
    uart_rx_interrupt(UART_INDEX, 1);


    //USB���ݳ�ʼ��
    usbStr.counter = 0;
    usbStr.receiveFinished = false;
    usbStr.receiveStart = false;
    usbStr.receiveIndex = 0;
    usbStr.connected = false;
    usbStr.inspectorEnable = false;
    usbStr.edgeboardCOM = false; //�Ƿ��ں�edageboardͨ��
}


/**
* @brief        USB-Edgeboard����һ���ֽ�����
* @param
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_TransmitByte(uint8_t data)
{
    uart_write_byte(UART_INDEX,data);

}

/**
* @brief        USB/UART�����жϺ���
* @param
* @ref
* @author       Leo
* @note
**/
uint8 USART_RX_BUF[USART_REC_LEN];
uint16 USART_RX_STA=0;
void uart_rx_interrupt_handler (void)
{


    uint8_t Uart1Res;
    uart_query_byte(UART_INDEX, &Uart1Res);   // �������� ��ѯʽ �����ݻ᷵�� TRUE û�����ݻ᷵�� FALSE

    if(!usbStr.edgeboardCOM)
    {
        if((USB_RX_STA&0x8000)==0)//����δ���       ���USB_RX_STA��uint16����λΪ0
        {
            if(USB_RX_STA&0x4000)//���յ���0x0d     �ڶ�λΪ1
                {
                if(Uart1Res!=0x0a)USB_RX_STA=0;//���մ���,���¿�ʼ
                else
                {
                    USB_RX_STA|=0x8000;  //���������
                    if((USB_RX_STA&0x3fff)==0)USB_RX_STA=0;
                }
                }
            else //��û�յ�0X0D
            {
            if(Uart1Res==0x0d)USB_RX_STA|=0x4000;
            else
                {
                rec_buff[USB_RX_STA&0X3FFF]=Uart1Res ;
                USB_RX_STA++;
                if(USB_RX_STA>(USART_REC_LEN-1))USB_RX_STA=0;//�������ݴ���,���¿�ʼ����
                }
            }
        }
    }

    if(Uart1Res == USB_FRAME_HEAD && !usbStr.receiveStart)//���֡ͷ
    {
       USB_RX_STA=0;
       usbStr.edgeboardCOM = true;
       usbStr.receiveStart = true;
       usbStr.receiveBuff[0] = Uart1Res;
       usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
       usbStr.receiveIndex = 1;
    }
    else if(usbStr.receiveIndex == 2)   //����֡����
    {
        usbStr.receiveBuff[usbStr.receiveIndex] = Uart1Res;
        usbStr.receiveIndex++;

        if(Uart1Res > USB_FRAME_LENMAX || Uart1Res < USB_FRAME_LENMIN) //֡������
        {
            usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
            usbStr.receiveIndex = 0;
            usbStr.receiveStart = false;
        }
    }
    else if(usbStr.receiveStart && usbStr.receiveIndex < USB_FRAME_LENMAX)
    {
        usbStr.receiveBuff[usbStr.receiveIndex] = Uart1Res;
        usbStr.receiveIndex++;
    }
    //����֡���
    if((usbStr.receiveIndex >= USB_FRAME_LENMAX || usbStr.receiveIndex >= usbStr.receiveBuff[2]) && usbStr.receiveIndex > USB_FRAME_LENMIN)
    {
        uint8_t check = 0;
        uint8_t length = USB_FRAME_LENMIN;



        length = usbStr.receiveBuff[2];
        for(int i=0;i<length-1;i++)
            check += usbStr.receiveBuff[i];

        if(check == usbStr.receiveBuff[length-1])//У��λ
        {
            memcpy(usbStr.receiveBuffFinished,usbStr.receiveBuff,USB_FRAME_LENMAX);
            usbStr.receiveFinished = true;

            //���ܳ�����ָ�����⴦������ʵʱ�ԣ�
            if(USB_ADDR_CONTROL  == usbStr.receiveBuffFinished[1])
            {
                Bint16_Union bint16_Union;
                Bint32_Union bint32_Union;
                for(int i=0;i<4;i++)
                    bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[3+i];

                bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[7];
                bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[8];

                SERVO_SetPwmValueCorrect(bint16_Union.U16);
                icarStr.ServoPwmSet = bint16_Union.U16;         //����
                icarStr.SpeedSet = bint32_Union.Float;          //�ٶ�
            }

            if(!usbStr.connected)//��λ����������ͨ��
            {
                //RGB_SetAllColor(RGB_COLOR_GREEN);
                GPIO_BuzzerEnable(BuzzerOk);
                usbStr.connected = true;
            }

            usbStr.counterDrop = 0;
        }

        usbStr.receiveIndex = 0;
        usbStr.receiveStart = false;
        usbStr.edgeboardCOM = false;
    }


}
IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart0_handle);
    uart_rx_interrupt_handler();
#if DEBUG_UART_USE_INTERRUPT                        // ������� debug �����ж�
        debug_interrupr_handler();                  // ���� debug ���ڽ��մ����� ���ݻᱻ debug ���λ�������ȡ
#endif                                              // ����޸��� DEBUG_UART_INDEX ����δ�����Ҫ�ŵ���Ӧ�Ĵ����ж�ȥ

}

/**
* @brief        �������߳̿�����
* @param
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Timr(void)
{
    if(usbStr.connected)//USBͨ�ŵ��߼��
    {
        usbStr.counterDrop++;
        if(usbStr.counterDrop >500)//3s
        {
            usbStr.connected = false;
            usbStr.inspectorEnable = false;
            icarStr.selfcheckEnable = false;
        }

        if(usbStr.inspectorEnable)
        {
            usbStr.counterSend++;
        }
    }
}


/**
* @brief        USBͨ�Ŵ�����
* @param
* @ref
* @author       Leo
* @note
**/

void USB_Edgeboard_Handle(void)
{
    if(usbStr.receiveFinished)                                                              //���ճɹ�
    {
        usbStr.receiveFinished = false;
        Bint32_Union bint32_Union;
        Bint16_Union bint16_Union;

        if(usbStr.receiveBuffFinished[1] & 0x80)    //������
        {
            uint8_t Addr = (uint8_t)(usbStr.receiveBuffFinished[1] & 0x7F);
            switch(Addr)
            {
                case USB_ADDR_BATTERY :             //�����Ϣ
                    break;

                case USB_ADDR_SERVOTHRESHOLD :      //�����ֵ
                    break;
            }
        }
        else //д����
        {
            switch(usbStr.receiveBuffFinished[1])
            {
                case USB_ADDR_SERVOTHRESHOLD :   //�����ֵ
                    if(usbStr.receiveBuffFinished[3] == 1)          //��ת��ֵ
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdLeft = bint16_Union.U16;
                        flashSaveEnable = true; //�ȴ�Flash�洢
                        SERVO_SetPwmValue(servoStr.thresholdLeft);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    else if(usbStr.receiveBuffFinished[3] == 2)     //��ת��ֵ
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdRight = bint16_Union.U16;
                        flashSaveEnable = true; //�ȴ�Flash�洢
                        SERVO_SetPwmValue(servoStr.thresholdRight);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    else if(usbStr.receiveBuffFinished[3] == 3)     //��ֵ
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdMiddle = bint16_Union.U16;
                        flashSaveEnable = true; //�ȴ�Flash�洢
                        SERVO_SetPwmValue(servoStr.thresholdMiddle);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    break;

                case USB_ADDR_BUZZER :      //��������Ч
                    if(usbStr.receiveBuffFinished[3] == 1)          //OK
                        GPIO_BuzzerEnable(BuzzerOk);
                    else if(usbStr.receiveBuffFinished[3] == 2)     //Warnning
                        GPIO_BuzzerEnable(BuzzerWarnning);
                    else if(usbStr.receiveBuffFinished[3] == 3)     //Finish
                        GPIO_BuzzerEnable(BuzzerFinish);
                    else if(usbStr.receiveBuffFinished[3] == 4)     //Ding
                        GPIO_BuzzerEnable(BuzzerDing);
                    else if(usbStr.receiveBuffFinished[3] == 5)     //SystemStart
                        GPIO_BuzzerEnable(BuzzerSysStart);

                    break;

                case USB_ADDR_LIGHT :         //LED��Ч
                    for(int i=0;i<4;i++)
                        bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[i+3];
//
//                    RGB_SetAllColor((unsigned long)bint32_Union.U32);
//                    rgbStr.lastColor = (unsigned long)bint32_Union.U32;

                    break;

                case USB_ADDR_SPEEDMODE:        //�ٿ�ģʽ�л�
                    if(usbStr.receiveBuffFinished[3] == 1)    //����ģʽ
                        motorStr.CloseLoop = false;
                    else
                        motorStr.CloseLoop = true;

                    icarStr.SpeedSet = 0;
                    GPIO_BuzzerEnable(BuzzerDing);
                    break;


                //-----------------------------[�Լ�������]-------------------------------------------
                case USB_ADDR_INSPECTOR :           //�Լ��������
                    usbStr.inspectorEnable = true;
                    break;

                case USB_ADDR_SELFCHECK :           //��ʼ�Լ�
                    ICAR_SelfcheckControl(usbStr.receiveBuffFinished[3]);
                    break;
            }

        }
    }


    //-----------------------[�Լ�������ݷ���]-----------------------------
    if(usbStr.inspectorEnable && usbStr.connected && usbStr.counterSend > 150)//150ms
    {
        USB_Edgeboard_ServoThreshold(1);        //���Ͷ����ֵ
        Delay_Ms(1);
        USB_Edgeboard_ServoThreshold(2);
        Delay_Ms(1);
        USB_Edgeboard_ServoThreshold(3);
        Delay_Ms(1);
        USB_Edgeboard_BatteryInfo();            //���͵����Ϣ
        Delay_Ms(1);
        USB_Edgeboard_CarSpeed();               //���ͳ���
        usbStr.counterSend = 0;
    }
}

/**
* @brief        USB���Ͱ����ź�
* @param        time: ����ʱ��
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_TransmitKey(uint16_t time)
{
    uint8_t check = 0;
    uint8_t buff[8];
    Bint16_Union bint16_Union;

    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_KEYINPUT; //��ַ
    buff[2] = 0x06; //֡��

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];

    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

    for(int i=0;i<8;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

void USB_Edgeboard_PDparam(uint16_t time)
{
    uint8_t check = 0;
    uint8_t buff[8];
    Bint16_Union bint16_Union;

    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_KEYINPUT; //��ַ
    buff[2] = 0x06; //֡��

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];

    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

    for(int i=0;i<8;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}









//----------------------------------------------[UNIT-���������Լ����ͨ�����ݣ��˲���δ��Դ��]----------------------------------------------------------
/**
* @brief        ���Ͷ����ֵ
* @param        chanel: 1/��ת��ֵ��2/��ת��ֵ��3/��ֵ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_ServoThreshold(uint8_t chanel)
{
    if(chanel<1 || chanel>3)
        return;

    Bint16_Union bint16_Union;
    uint8_t check = 0;
    uint8_t buff[9];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_SERVOTHRESHOLD; //��ַ
    buff[2] = 0x07; //֡��
    buff[3] = chanel; //ͨ��

    switch(chanel)
    {
        case 1:
        {
            bint16_Union.U16 = servoStr.thresholdLeft;
            buff[4] = bint16_Union.U8_Buff[0];
            buff[5] = bint16_Union.U8_Buff[1];
            break;
        }
        case 2:
        {
            bint16_Union.U16 = servoStr.thresholdRight;
            buff[4] = bint16_Union.U8_Buff[0];
            buff[5] = bint16_Union.U8_Buff[1];
            break;
        }
        case 3:
        {
            bint16_Union.U16 = servoStr.thresholdMiddle;
            buff[4] = bint16_Union.U8_Buff[0];
            buff[5] = bint16_Union.U8_Buff[1];
            break;
        }
    }

    for(int i=0;i<6;i++)
        check += buff[i];

    buff[6] = check;

    for(int i=0;i<9;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

/**
* @brief        ���Ͱ�����Ӧ��Ϣ
* @param        time: ����ʱ��/ms
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_KeyPress(uint16_t time)
{
    if(time<100)
        return;

    Bint16_Union bint16_Union;
    uint8_t check = 0;
    uint8_t buff[8];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_KEYINPUT; //��ַ
    buff[2] = 0x06; //֡��

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];

    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

    for(int i=0;i<8;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}


/**
* @brief        ���͵����Ϣ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_BatteryInfo(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[11];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_BATTERY; //��ַ
    buff[2] = 0x09; //֡��

    buff[3] = icarStr.Electricity; //����
    bint32_Union.Float = icarStr.Voltage;
    buff[4] = bint32_Union.U8_Buff[0];
    buff[5] = bint32_Union.U8_Buff[1];
    buff[6] = bint32_Union.U8_Buff[2];
    buff[7] = bint32_Union.U8_Buff[3];

    for(int i=0;i<8;i++)
        check += buff[i];

    buff[8] = check;

    for(int i=0;i<11;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

/**
* @brief        ���ͳ�����Ϣ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_CarSpeed(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[10];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_SPEEDBACK; //��ַ
    buff[2] = 0x08; //֡��

    bint32_Union.Float = icarStr.SpeedFeedback;
    buff[3] = bint32_Union.U8_Buff[0];
    buff[4] = bint32_Union.U8_Buff[1];
    buff[5] = bint32_Union.U8_Buff[2];
    buff[6] = bint32_Union.U8_Buff[3];

    for(int i=0;i<7;i++)
        check += buff[i];

    buff[7] = check;

    for(int i=0;i<10;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

/**
* @brief        �����Լ���Ϣ
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Selfcheck(uint8_t step)
{
    Bint16_Union bint16_Union;
    uint8_t check = 0;
    uint8_t buff[9];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_SELFCHECK; //��ַ
    buff[2] = 0x07; //֡��

    buff[3] = step;

    bint16_Union.U16 = icarStr.errorCode;
    buff[4] = bint16_Union.U8_Buff[0];
    buff[5] = bint16_Union.U8_Buff[1];

    for(int i=0;i<6;i++)
        check += buff[i];

    buff[6] = check;

    for(int i=0;i<9;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}

//------------------------------------------------[END]-------------------------------------------------------------

