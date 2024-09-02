#ifndef __PID_H
#define __PID_H


#include "main.h"
//#include "zf_common_headfile.h"

/******************************************************************************/
#define VV_DEADLINE             0.1                      //�ٶ�PID������������Χ����������ֹǿӲ�̶�
#define PID_VKP                 80.0f                    //PID�궨ֵ
#define PID_VKI                 20.0f                    //PID�궨ֵ
#define PID_VKD                 10.0f                   //PID�궨ֵ

/**
* @brief    PID���
**/
typedef struct
{
    float vi_Ref;                       //�ٶ�PID���ٶ��趨ֵ
    float vi_FeedBack;                  //�ٶ�PID���ٶȷ���ֵ
    float vi_PreError;                  //�ٶ�PID���ٶ����,vi_Ref - vi_FeedBack
    float vi_PreDerror;                 //�ٶ�PID��ǰһ�Σ��ٶ����֮�d_error-PreDerror;
    float v_Kp;                         //����ϵ����Kp = Kp
    float v_Ki;                         //����ϵ����Ki = Kp * ( T / Ti )
    float v_Kd;                         //΢��ϵ����Kd = KP * Td * T
    float vl_PreU;                      //PID���ֵ
}PIDStruct;

extern PIDStruct pidStr;

void PID_Init(void);
signed int PID_MoveCalculate(PIDStruct *pp);

#endif



