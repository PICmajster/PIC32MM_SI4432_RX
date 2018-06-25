/**
  EXT_INT Generated Driver API Header File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    ext_int.h

    @Description:
    This header file provides APIs for driver for EXT_INT. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : 1.55
        Device            :  PIC32MM0256GPM048
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.05
        MPLAB             :  MPLAB X v4.15
*/


#ifndef _EXT_INT_H
#define _EXT_INT_H


#include <xc.h>

#define EX_INT2_InterruptFlagClear()       IFS0CLR= 1 << _IFS0_INT2IF_POSITION
#define EX_INT2_InterruptDisable()     (IEC0bits.INT2IE = 0)
#define EX_INT2_InterruptEnable()       (IEC0bits.INT2IE = 1)
#define EX_INT2_NegativeEdgeSet()          (INTCONCLR = _INTCON_INT2EP_MASK)
#define EX_INT2_PositiveEdgeSet()          (INTCONSET = _INTCON_INT2EP_MASK)

void EXT_INT_Initialize(void);
extern volatile uint8_t nIRQ_flag;

#endif
