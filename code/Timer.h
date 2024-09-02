#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#define PIT_NUM                 (CCU61_CH0 )                                     // 使用的周期中断编号
void Timer_Init(void);
void cc61_pit_ch0_isr(void);
#endif


