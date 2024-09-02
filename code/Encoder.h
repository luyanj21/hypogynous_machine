#ifndef __ENCODER_H
#define __ENCODER_H


#include "main.h"
//#include "zf_common_headfile.h"

#define ENCODER_1                   (TIM2_ENCODER)
#define ENCODER_1_A                 (TIM2_ENCODER_CH1_P33_7)
#define ENCODER_1_B                 (TIM2_ENCODER_CH2_P33_6)

void ENCODER_Init(void);
void ENCODER_RevSample (void);

//luy 电机测试用的
//void cc60_pit_ch0_isr (void);

#endif



