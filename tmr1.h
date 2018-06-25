/*
    File Name:        :  tmr1.h

    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/

#ifndef _TMR1_H
#define _TMR1_H


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h> /*dyrektywy do uint8_t itp*/


#define TMR1_INTERRUPT_TICKER_FACTOR    1
extern volatile uint8_t Timer1_Programowy , Timer2_Programowy ;

void TMR1_Initialize (void);
void TMR1_Start( void );
void TMR1_Stop( void );


#endif
    
/*End of File*/
