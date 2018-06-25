/*
    File Name:        :  main.c
    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/
#include "mcu_config_files/mcc.h"
#include "delay.h"
#include "dogm162.h"
#include "si4432.h"

char napis[] = "TEST SI4432 RX";
uint8_t RXbufor[13];
uint8_t ItStatus1, ItStatus2, rssi, length, ix;

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    lcd_Initialize();
    SPI2_Initialize(); /*Master*/
    PORTCbits.RC6 = 1;
    SI4432_Initialize();
    
    // When using interrupts, you need to set the Global Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
       INTERRUPT_GlobalEnable();
       TMR1_Stop();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalDisable();
    lcd_Locate(1,1);
    lcd_String(napis);
    
    /*Read interrupt status registers. It clear all pending interrupts and the nIRQ pin goes back to high*/
    ItStatus1 = SpiReadRegister(SI4432_REG_03_INTERRUPT_STATUS1);/*read the Interrupt Status1 register*/
    ItStatus2 = SpiReadRegister(SI4432_REG_04_INTERRUPT_STATUS2);/*read the Interrupt Status2 register*/ 
        
    /*enable transmitter
     * Ready Mode - ON,
     * RX on in Manual Transmit Mode. (Automatically cleared if Multiple Packets config. is disabled and a valid packet received.)
    */
    SpiWriteRegister(SI4432_REG_07_OPERATING_MODE1, 0x05); 
    /*waiting for the packet has flown to the receiver*/
    
    while (1)
    {
        if(!Timer1_Programowy) {
              Timer1_Programowy = 25 ; /*Timer1 sprzetowy x Timer1_Programowy = 100ms x 15 = 2,5 s*/
              lcd_Locate(2,1);
              lcd_String("                ");
              TMR1_Stop();
               
              }    
            
        
        if(nIRQ_flag){
        /*read interrupt status registers*/
        ItStatus1 = SpiReadRegister(SI4432_REG_03_INTERRUPT_STATUS1);/*read the Interrupt Status1 register*/
        ItStatus2 = SpiReadRegister(SI4432_REG_04_INTERRUPT_STATUS2);/*read the Interrupt Status2 register*/
             
            /*if Sync Word Received interrupt occurred*/
            if( (ItStatus2 & SI4432_ENSWDET) == SI4432_ENSWDET ){
              rssi = SpiReadRegister(SI4432_REG_26_RSSI);/*read the RSSI*/
               
             }
            /*if Valid Packet Received interrupt occurred*/
            if( (ItStatus1 & SI4432_IPKVALID) == SI4432_IPKVALID ){
               SpiWriteRegister(SI4432_REG_07_OPERATING_MODE1, 0x01); //disable the receiver chain
               length = SpiReadRegister(SI4432_REG_4B_RECEIVED_PACKET_LENGTH);/*read the Received Packet Length register*/
                                                           
               for(ix=0;ix < length;ix++)
                {
                    RXbufor[ix] = SpiReadRegister(SI4432_REG_7F_FIFO_ACCESS);/*read the FIFO Access register*/
                }
               lcd_Locate(2,1);
               lcd_String("                ");
               lcd_Locate(2,1);
               lcd_String(RXbufor);
               lcd_Locate(2,13);
               lcd_Integer(rssi);
                              
            }
        nIRQ_flag = 0;
        /*reset the RX FIFO*/
        SpiWriteRegister(SI4432_REG_08_OPERATING_MODE2, 0x02);
        SpiWriteRegister(SI4432_REG_08_OPERATING_MODE2, 0x00);
        /*enable transmitter
         Ready Mode - ON,
         RX on in Manual Transmit Mode. (Automatically cleared if Multiple Packets config. is disabled and a valid packet received.)
         */
        SpiWriteRegister(SI4432_REG_07_OPERATING_MODE1, 0x05); 
        TMR1_Start();       
        }
    }
}

