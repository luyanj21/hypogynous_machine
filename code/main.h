/*
 * main.h
 *
 *  Created on: 2024Äê3ÔÂ14ÈÕ
 *      Author: cyl
 */

#ifndef __MAIN_H__
#define __MAIN_H__
/************************ System *****************************/
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "zf_common_headfile.h"


//typedef enum { false = 0,true = 1} bool;
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

typedef union
{
    uint8 U8_Buff[2];
    uint16 U16;
    int16 S16;
}Bint16_Union;

typedef union
{
    uint8 U8_Buff[4];
    float Float;
    unsigned long U32;
}Bint32_Union;

#include "Delay.h"
#include "Encoder.h"
#include "Gpio.h"
#include "Pid.h"
#include "Motor.h"
#include "Flash.h"
//#include "Soc.h"
#include "Timer.h"
#include "Icar.h"
#include "Servo.h"
//#include "Rgb.h"
#include "Usb.h"

#endif /* LIBRARIES_SASU_CARDO_MAIN_H_ */
