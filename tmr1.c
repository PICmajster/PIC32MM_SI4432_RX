/*
    File Name:        :  tmr1.c

    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/

#include <xc.h>
#include "tmr1.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /*dyrektywy do uint8_t itp*/

volatile uint8_t Timer1_Programowy , Timer2_Programowy ;

void TMR1_Initialize (void)
{
    uint32_t tcon_value = 0x00000000;
    //  TCKPS 1:64; TWDIS disabled; TCS PBCLK; TECS SOSC; SIDL disabled; TSYNC disabled; TWIP disabled; TGATE disabled; ON enabled; 
    T1CON = 0x8020;   
    tcon_value = 0x8020;  // Temporary storage of value
    T1CONCLR = _T1CON_ON_MASK;  // disable Timer, before loading the period/counter value
    // Period = 0.1 s; Frequency = 24000000 Hz; PR1 37500; 
    PR1 = 0x927C ;

    T1CON = tcon_value;//restore the TCON value
    IFS0CLR= 1 << _IFS0_T1IF_POSITION;
    IEC0bits.T1IE = true;
  
}



void __attribute__ ((vector(_TIMER_1_VECTOR), interrupt(IPL2SOFT))) TMR1_ISR()
{
    uint8_t x;
    x = Timer1_Programowy ;
    if (x) Timer1_Programowy = --x ;
    x = Timer2_Programowy ;
    if (x) Timer2_Programowy = --x ;
    /*zerowanie flagi przerwania*/
    IFS0CLR= 1 << _IFS0_T1IF_POSITION; /*kasujemy flagusie od przerwania*/
}


void TMR1_Start( void )
{
    /*Clear atomic TMR1*/
    TMR1CLR = 0xFFF ;
    /*Enable the interrupt*/
    IEC0bits.T1IE = true;

    /* Start the Timer */
    T1CONSET = _T1CON_ON_MASK;
}

void TMR1_Stop( void )
{
    /* Stop the Timer */
    T1CONCLR = _T1CON_ON_MASK;

    /*Disable the interrupt*/
    IEC0bits.T1IE = false;
}

/*End of File*/
