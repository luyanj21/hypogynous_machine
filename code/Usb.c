#include "Usb.h"
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

UsbStruct usbStr;
uint8 rec_buff[USART_REC_LEN];
uint16 USB_RX_STA = 0;  //接收状态标志位
/**
* @brief        USB/UART初始化
* @param
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Init(void)
{
    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);
    uart_rx_interrupt(UART_INDEX, 1);


    //USB数据初始化
    usbStr.counter = 0;
    usbStr.receiveFinished = false;
    usbStr.receiveStart = false;
    usbStr.receiveIndex = 0;
    usbStr.connected = false;
    usbStr.inspectorEnable = false;
    usbStr.edgeboardCOM = false; //是否处于和edageboard通信
}


/**
* @brief        USB-Edgeboard发送一个字节数据
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
* @brief        USB/UART接收中断函数
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
    uart_query_byte(UART_INDEX, &Uart1Res);   // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE

    if(!usbStr.edgeboardCOM)
    {
        if((USB_RX_STA&0x8000)==0)//接收未完成       检查USB_RX_STA（uint16）首位为0
        {
            if(USB_RX_STA&0x4000)//接收到了0x0d     第二位为1
                {
                if(Uart1Res!=0x0a)USB_RX_STA=0;//接收错误,重新开始
                else
                {
                    USB_RX_STA|=0x8000;  //接收完成了
                    if((USB_RX_STA&0x3fff)==0)USB_RX_STA=0;
                }
                }
            else //还没收到0X0D
            {
            if(Uart1Res==0x0d)USB_RX_STA|=0x4000;
            else
                {
                rec_buff[USB_RX_STA&0X3FFF]=Uart1Res ;
                USB_RX_STA++;
                if(USB_RX_STA>(USART_REC_LEN-1))USB_RX_STA=0;//接收数据错误,重新开始接收
                }
            }
        }
    }

    if(Uart1Res == USB_FRAME_HEAD && !usbStr.receiveStart)//监测帧头
    {
       USB_RX_STA=0;
       usbStr.edgeboardCOM = true;
       usbStr.receiveStart = true;
       usbStr.receiveBuff[0] = Uart1Res;
       usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
       usbStr.receiveIndex = 1;
    }
    else if(usbStr.receiveIndex == 2)   //接收帧长度
    {
        usbStr.receiveBuff[usbStr.receiveIndex] = Uart1Res;
        usbStr.receiveIndex++;

        if(Uart1Res > USB_FRAME_LENMAX || Uart1Res < USB_FRAME_LENMIN) //帧长错误
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
    //接收帧完毕
    if((usbStr.receiveIndex >= USB_FRAME_LENMAX || usbStr.receiveIndex >= usbStr.receiveBuff[2]) && usbStr.receiveIndex > USB_FRAME_LENMIN)
    {
        uint8_t check = 0;
        uint8_t length = USB_FRAME_LENMIN;



        length = usbStr.receiveBuff[2];
        for(int i=0;i<length-1;i++)
            check += usbStr.receiveBuff[i];

        if(check == usbStr.receiveBuff[length-1])//校验位
        {
            memcpy(usbStr.receiveBuffFinished,usbStr.receiveBuff,USB_FRAME_LENMAX);
            usbStr.receiveFinished = true;

            //智能车控制指令特殊处理（保障实时性）
            if(USB_ADDR_CONTROL  == usbStr.receiveBuffFinished[1])
            {
                Bint16_Union bint16_Union;
                Bint32_Union bint32_Union;
                for(int i=0;i<4;i++)
                    bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[3+i];

                bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[7];
                bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[8];

                SERVO_SetPwmValueCorrect(bint16_Union.U16);
                icarStr.ServoPwmSet = bint16_Union.U16;         //方向
                icarStr.SpeedSet = bint32_Union.Float;          //速度
            }

            if(!usbStr.connected)//上位机初次连接通信
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
    interrupt_global_enable(0);                     // 开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart0_handle);
    uart_rx_interrupt_handler();
#if DEBUG_UART_USE_INTERRUPT                        // 如果开启 debug 串口中断
        debug_interrupr_handler();                  // 调用 debug 串口接收处理函数 数据会被 debug 环形缓冲区读取
#endif                                              // 如果修改了 DEBUG_UART_INDEX 那这段代码需要放到对应的串口中断去

}

/**
* @brief        监测软件线程控制器
* @param
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Timr(void)
{
    if(usbStr.connected)//USB通信掉线检测
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
* @brief        USB通信处理函数
* @param
* @ref
* @author       Leo
* @note
**/

void USB_Edgeboard_Handle(void)
{
    if(usbStr.receiveFinished)                                                              //接收成功
    {
        usbStr.receiveFinished = false;
        Bint32_Union bint32_Union;
        Bint16_Union bint16_Union;

        if(usbStr.receiveBuffFinished[1] & 0x80)    //读数据
        {
            uint8_t Addr = (uint8_t)(usbStr.receiveBuffFinished[1] & 0x7F);
            switch(Addr)
            {
                case USB_ADDR_BATTERY :             //电池信息
                    break;

                case USB_ADDR_SERVOTHRESHOLD :      //舵机阈值
                    break;
            }
        }
        else //写数据
        {
            switch(usbStr.receiveBuffFinished[1])
            {
                case USB_ADDR_SERVOTHRESHOLD :   //舵机阈值
                    if(usbStr.receiveBuffFinished[3] == 1)          //左转阈值
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdLeft = bint16_Union.U16;
                        flashSaveEnable = true; //等待Flash存储
                        SERVO_SetPwmValue(servoStr.thresholdLeft);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    else if(usbStr.receiveBuffFinished[3] == 2)     //右转阈值
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdRight = bint16_Union.U16;
                        flashSaveEnable = true; //等待Flash存储
                        SERVO_SetPwmValue(servoStr.thresholdRight);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    else if(usbStr.receiveBuffFinished[3] == 3)     //中值
                    {
                        bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[4];
                        bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[5];
                        servoStr.thresholdMiddle = bint16_Union.U16;
                        flashSaveEnable = true; //等待Flash存储
                        SERVO_SetPwmValue(servoStr.thresholdMiddle);
                        GPIO_BuzzerEnable(BuzzerDing);
                    }
                    break;

                case USB_ADDR_BUZZER :      //蜂鸣器音效
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

                case USB_ADDR_LIGHT :         //LED灯效
                    for(int i=0;i<4;i++)
                        bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[i+3];
//
//                    RGB_SetAllColor((unsigned long)bint32_Union.U32);
//                    rgbStr.lastColor = (unsigned long)bint32_Union.U32;

                    break;

                case USB_ADDR_SPEEDMODE:        //速控模式切换
                    if(usbStr.receiveBuffFinished[3] == 1)    //开环模式
                        motorStr.CloseLoop = false;
                    else
                        motorStr.CloseLoop = true;

                    icarStr.SpeedSet = 0;
                    GPIO_BuzzerEnable(BuzzerDing);
                    break;


                //-----------------------------[自检软件相关]-------------------------------------------
                case USB_ADDR_INSPECTOR :           //自检软件心跳
                    usbStr.inspectorEnable = true;
                    break;

                case USB_ADDR_SELFCHECK :           //开始自检
                    ICAR_SelfcheckControl(usbStr.receiveBuffFinished[3]);
                    break;
            }

        }
    }


    //-----------------------[自检软件数据发送]-----------------------------
    if(usbStr.inspectorEnable && usbStr.connected && usbStr.counterSend > 150)//150ms
    {
        USB_Edgeboard_ServoThreshold(1);        //发送舵机阈值
        Delay_Ms(1);
        USB_Edgeboard_ServoThreshold(2);
        Delay_Ms(1);
        USB_Edgeboard_ServoThreshold(3);
        Delay_Ms(1);
        USB_Edgeboard_BatteryInfo();            //发送电池信息
        Delay_Ms(1);
        USB_Edgeboard_CarSpeed();               //发送车速
        usbStr.counterSend = 0;
    }
}

/**
* @brief        USB发送按键信号
* @param        time: 按键时长
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_TransmitKey(uint16_t time)
{
    uint8_t check = 0;
    uint8_t buff[8];
    Bint16_Union bint16_Union;

    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_KEYINPUT; //地址
    buff[2] = 0x06; //帧长

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

    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_KEYINPUT; //地址
    buff[2] = 0x06; //帧长

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];

    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

    for(int i=0;i<8;i++)
        USB_Edgeboard_TransmitByte(buff[i]);
}









//----------------------------------------------[UNIT-智能汽车自检软件通信内容（此部分未开源）]----------------------------------------------------------
/**
* @brief        发送舵机阈值
* @param        chanel: 1/左转阈值，2/右转阈值，3/中值
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
    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_SERVOTHRESHOLD; //地址
    buff[2] = 0x07; //帧长
    buff[3] = chanel; //通道

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
* @brief        发送按键响应信息
* @param        time: 按下时长/ms
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
    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_KEYINPUT; //地址
    buff[2] = 0x06; //帧长

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
* @brief        发送电池信息
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_BatteryInfo(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[11];
    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_BATTERY; //地址
    buff[2] = 0x09; //帧长

    buff[3] = icarStr.Electricity; //电量
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
* @brief        发送车速信息
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_CarSpeed(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[10];
    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_SPEEDBACK; //地址
    buff[2] = 0x08; //帧长

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
* @brief        发送自检信息
* @ref
* @author       Leo
* @note
**/
void USB_Edgeboard_Selfcheck(uint8_t step)
{
    Bint16_Union bint16_Union;
    uint8_t check = 0;
    uint8_t buff[9];
    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_SELFCHECK; //地址
    buff[2] = 0x07; //帧长

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

