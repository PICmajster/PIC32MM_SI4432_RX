/**
  EXT_INT Generated Driver File 
  
  @File Name:
    ext_int.c

  
  @Description:
    This source file provides implementations for driver APIs for EXT_INT. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : 1.55
        Device            :  PIC32MM0256GPM048
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.05
        MPLAB             :  MPLAB X v4.15
*/


/**
   Section: Includes
 */
#include <xc.h>
#include "ext_int.h"

volatile uint8_t nIRQ_flag = 0;

/**
  Interrupt Handler for EX_INT2 - INT2
*/
void __attribute__ ( ( vector ( _EXTERNAL_2_VECTOR ), interrupt ( IPL1SOFT ) ) ) _EXTERNAL_2_ISR (void)
{
    nIRQ_flag = 1 ;
    PORTBbits.RB6 = 1; /*LED ON*/ 
    EX_INT2_InterruptFlagClear();
}

/**
    Section: External Interrupt Initializers
 */
/**
    void EXT_INT_Initialize(void)

    Initializer for the following external interrupts
    INT2
*/
void EXT_INT_Initialize(void)
{
    /*******
     * INT2
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT2_InterruptFlagClear();   
    EX_INT2_NegativeEdgeSet();
    EX_INT2_InterruptEnable();
    
}
