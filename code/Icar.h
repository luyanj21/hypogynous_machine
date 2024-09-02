#ifndef __SMARTCAR_H__
#define __SMARTCAR_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "main.h"
//#include "zf_common_headfile.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

/**
* @brief    ���ܳ��Լ첽��
**/
typedef enum
{
    Selfcheck_None = 0,             //��ʼ����
    Selfcheck_MotorA,               //�����ת����
    Selfcheck_MotorB,               //�����ת����
    Selfcheck_MotorC,               //�����ת����
    Selfcheck_MotorD,               //�����ת����
    Selfcheck_MotorE,               //����ջ���������
    Selfcheck_MotorF,               //����ջ���������
    Selfcheck_MotorG,               //����ջ���ת����
    Selfcheck_MotorH,               //����ջ���ת����
    Selfcheck_ServoA,               //�������A
    Selfcheck_Com,                  //ͨ�Ų���
    Selfcheck_Buzzer,               //����������
    Selfcheck_RgbLed,               //��Ч����
    Selfcheck_Key,                  //��������
    Selfcheck_Finish                //�������
}SelfcheckEnum;


/**
* @brief    ���ܳ����
**/
typedef struct                              //[���ܳ���������]
{
    float Voltage;                          //��ص�ѹ
    uint8_t Electricity;                    //��ص����ٷֱȣ�0~100
    float SpeedSet;                         //���Ŀ���ٶȣ�m/s
    float SpeedFeedback;                    //���ģ��ʵ���ٶȣ�m/s
    float SpeedMaxRecords;                  //���Լ�¼�����
    uint16_t ServoPwmSet;                   //���PWM����

    uint16_t counterKeyA;                   //����ģʽA������
    bool keyPressed;                        //��������
    bool sprintEnable;                      //�ջ����ʹ��
    uint16_t counterSprint;                 //�ջ����ʱ��
    uint16_t errorCode;                     //�������

    bool selfcheckEnable;                   //���ܳ��Լ�ʹ��
    uint16_t counterSelfcheck;              //�Լ������
    uint8_t timesSendStep;                  //���ͳ�ʱ���ݴ���
    uint16_t counterModuleCheck;            //�Լ������
    SelfcheckEnum selfcheckStep;            //�Լ첽��
    uint8_t speedSampleStep;                //�ٶȲ�������
}IcarStruct;


extern IcarStruct icarStr;


void ICAR_Init(void);
void ICAR_Timer(void);
void ICAR_Handle(void);
void ICAR_Reboot(void);

//�Լ����
void ICAR_Selfcheck(void);
void ICAR_SelfcheckControl(uint8_t step);
float ICAR_SpeedSample(void);
bool ICAR_ServoCheck(void);
bool ICAR_BuzzerCheck(void);
void ICAR_RgbCheck(void);
void ICAR_SortFloat(float *buff, uint8_t start, uint8_t len);
float CMATH_AbsFloat(float data);


#endif

//===========================================  End Of File  ===========================================//


