/*
    File Name:        :  si4432.c

    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
 */

#include "mcu_config_files/mcc.h"
#include "delay.h"
#include "si4432.h"


void SpiWriteRegister (uint8_t reg, uint8_t value)
{
/*Select the radio by pulling the nSEL pin to low*/
SI4432_nSEL = 0;                          
/*write the address of the register into the SPI buffer of the MCU
 (important to set the MSB bit)*/
SPI2_Exchange8bit(reg|SI4432_WRITE_MASK);
/*write the new value of the radio register into the SPI buffer of the MCU*/
SPI2_Exchange8bit(value);
/*Deselect the radio by pulling high the nSEL pin*/
SI4432_nSEL = 1;                            
}

uint8_t SpiReadRegister (uint8_t reg)
{
uint8_t rData;
/*Select the radio by pulling the nSEL pin to low*/
SI4432_nSEL = 0;
/*Write the address of the register into the SPI buffer of the MCU 
(important to clear the MSB bit)*/
SPI2_Exchange8bit(reg);
/*Write a dummy data byte into the SPI buffer of the MCU. During sending 
this byte the MCU will read the value of the radio register and save it 
in its SPI buffer.*/
rData = SPI2_Exchange8bit(0xFF); /*write dummy data into the SPI register*/
/*Deselect the radio by pulling high the nSEL pin*/
SI4432_nSEL = 1; 
/*Read the received radio register value and return with it*/
return rData;
}

void SI4432_Initialize (void){
    delayMs(15);
    /*Software Reset for the Radio*/
    SpiWriteRegister (SI4432_REG_07_OPERATING_MODE1, SI4432_SWRES);
    delayMs(1);
    
    /*Set RF Parameters*/
    
    /*set the center frequency to 434 MHz, see EZRadioPRO Si443x Register Calculator (B1 Silicon)*/
    SpiWriteRegister(SI4432_REG_75_FREQUENCY_BAND_SELECT, 0x53);        
    SpiWriteRegister(SI4432_REG_76_NOMINAL_CARRIER_FREQUENCY1, 0x64);   
    SpiWriteRegister(SI4432_REG_77_NOMINAL_CARRIER_FREQUENCY0 , 0x00); 
    
    /*set the desired TX data rate (2kbps)*/
    SpiWriteRegister(SI4432_REG_6E_TX_DATA_RATE1, 0x10);        
    SpiWriteRegister(SI4432_REG_6F_TX_DATA_RATE0, 0x62);        
    SpiWriteRegister(SI4432_REG_70_MODULATION_CONTROL1, 0x2C);  
    SpiWriteRegister(SI4432_REG_58_CHARGE_PUMP_CURRENT_TRIMMING, 0x80); 
    
    /*set the desired TX deviation (+-5 kHz)*/
    SpiWriteRegister(SI4432_REG_72_FREQUENCY_DEVIATION, 0x08);  
    SpiWriteRegister(SI4432_REG_71_MODULATION_CONTROL2, 0x23); 
    
   /*Modem settings GFSK for RX, AFC Enable*/
    SpiWriteRegister(SI4432_REG_1C_IF_FILTER_BANDWIDTH, 0x2B);        
    SpiWriteRegister(SI4432_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE, 0xF4);        
    SpiWriteRegister(SI4432_REG_21_CLOCK_RECOVERY_OFFSET2, 0x20);  
    SpiWriteRegister(SI4432_REG_22_CLOCK_RECOVERY_OFFSET1, 0x41); 
    SpiWriteRegister(SI4432_REG_23_CLOCK_RECOVERY_OFFSET0, 0x89);        
    SpiWriteRegister(SI4432_REG_24_CLOCK_RECOVERY_TIMING_LOOP_GAIN1, 0x00);        
    SpiWriteRegister(SI4432_REG_25_CLOCK_RECOVERY_TIMING_LOOP_GAIN0, 0x36);  
    SpiWriteRegister(SI4432_REG_1D_AFC_LOOP_GEARSHIFT_OVERRIDE, 0x44); 
    SpiWriteRegister(SI4432_REG_1E_AFC_TIMING_CONTROL, 0x0A);        
    SpiWriteRegister(SI4432_REG_2A_AFC_LIMITER, 0x1D);        
    SpiWriteRegister(SI4432_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE, 0x03);  
    SpiWriteRegister(SI4432_REG_69_AGC_OVERRIDE1, 0x60);
    
    /*Set TX Power -1dBm ..+20dBm step 3dBm / 0b000 minimum ...0b111 maximum*/
    SpiWriteRegister(SI4432_REG_6D_TX_POWER , SI4432_TX_POWER_11DBM);
    
    /* Select Turn On Packet Handler - ON , 
     * Select LSB/MSB First - MSB
     * Enable CRC
     * CRC Over Data Only
     * CRC TYPE - CRC16-IBM
     */
    SpiWriteRegister(SI4432_REG_30_DATA_ACCESS_CONTROL, 0xAD);
    
    /* No broadcast address enable
     * No Received Header check*/
    SpiWriteRegister(SI4432_REG_32_HEADER_CONTROL1 , 0x00);
    
    /*Synchronization Word 3 and 2, Disable header bytes*/
    SpiWriteRegister(SI4432_REG_33_HEADER_CONTROL2 , 0x02);
    
    /*Preamble Lenght 10 x 4 bits = 40 bits, AFC - ON*/
    SpiWriteRegister(SI4432_REG_34_PREAMBLE_LENGTH , 0x0A);
    
    /*Preamble Detection Threshold 5 x 4 bits = 20 bits
     *rssi_offset
     */
    SpiWriteRegister(SI4432_REG_35_PREAMBLE_DETECTION_CONTROL , 0x2A);
    
    /* Configure sync Word 3 Value = 2D
     * Configure sync Word 2 Value = D4
     */
    SpiWriteRegister(SI4432_REG_36_SYNC_WORD3 , 0x2D);
    SpiWriteRegister(SI4432_REG_37_SYNC_WORD2 , 0xD4);
    SpiWriteRegister(SI4432_REG_38_SYNC_WORD1 , 0x00);
    SpiWriteRegister(SI4432_REG_39_SYNC_WORD0 , 0x00);
    
    /*Transmit Header*/
    SpiWriteRegister(SI4432_REG_3A_TRANSMIT_HEADER3 , 0x00);
    SpiWriteRegister(SI4432_REG_3B_TRANSMIT_HEADER2  , 0x00);
    SpiWriteRegister(SI4432_REG_3C_TRANSMIT_HEADER1 , 0x00);
    SpiWriteRegister(SI4432_REG_3D_TRANSMIT_HEADER0 , 0x00);
    
    /*DATA Length Transmit*/
//    SpiWriteRegister(SI4432_REG_3E_PACKET_LENGTH , 0x08);
    
    /*Check Header*/
    SpiWriteRegister(SI4432_REG_3F_CHECK_HEADER3 , 0x00);
    SpiWriteRegister(SI4432_REG_40_CHECK_HEADER2  , 0x00);
    SpiWriteRegister(SI4432_REG_41_CHECK_HEADER1 , 0x00);
    SpiWriteRegister(SI4432_REG_42_CHECK_HEADER0 , 0x00);
    
    /*Header Enable*/
    SpiWriteRegister(SI4432_REG_43_HEADER_ENABLE3 , 0x00);
    SpiWriteRegister(SI4432_REG_44_HEADER_ENABLE2  , 0x00);
    SpiWriteRegister(SI4432_REG_45_HEADER_ENABLE1 , 0x00);
    SpiWriteRegister(SI4432_REG_46_HEADER_ENABLE0 , 0x00);
    
    /*Set IRQ
     Enable Valid Packet Received
     Enable CRC Error*/
    SpiWriteRegister(SI4432_REG_05_INTERRUPT_ENABLE1 , 0x03);
   
    /*Enable Sync Word Detected.*/
    SpiWriteRegister(SI4432_REG_06_INTERRUPT_ENABLE2 , 0x80);
    
//    /*fill the payload into the transmit FIFO, 8 bytes*/
//    //write 0x42 ('B') to the FIFO Access register
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x42);
//    //write 0x55 ('U') to the FIFO Access register
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x55);
//   //write 0x54 ('T') to the FIFO Access register
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x54);
//    //write 0x54 ('T') to the FIFO Access register
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x54);
//     //write 0x4F ('O') to the FIFO Access register
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x4F);
//    //write 0x4E ('N') to the FIFO Access register
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x4E);
//    //write 0x31 ('1') to the FIFO Access register
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x31);
//    //write 0x0D (CR) to the FIFO Access register 
//    SpiWriteRegister(SI4432_REG_7F_FIFO_ACCESS , 0x0D);
   
    
}

void SI4432_ResetTxFifo(void)
{
	SpiWriteRegister(SI4432_REG_08_OPERATING_MODE2 , SI4432_FFCLRTX);
}

void SI4432_ResetRxFifo(void)
{
	SpiWriteRegister(SI4432_REG_08_OPERATING_MODE2 , SI4432_FFCLRRX);
}

void SI4432_SetMode(SI4432ModeEnum mode)
{
	uint8_t reg_value = 0;

		switch(mode) {
			case SI4432_ModeIdle	: reg_value = SI4432_XTON;
            break;
			case SI4432_ModeStandby	: reg_value = 0x00;
            break;
			case SI4432_ModeSleep	: reg_value = SI4432_ENWT;
            break;
			case SI4432_ModeTune	: reg_value = SI4432_PLLON;
            break;
			case SI4432_ModeTx      : reg_value = SI4432_TXON;
            break;
			case SI4432_ModeRx      : reg_value = SI4432_RXON;
            break;
			default                 : return;
		}
		SpiWriteRegister(SI4432_REG_07_OPERATING_MODE1, reg_value);
}

void SI4432_SetTxPower(uint8_t values)
{
   SpiWriteRegister(SI4432_REG_6D_TX_POWER , values);
   /*values :
    SI4432_TX_POWER_1DBM
    SI4432_TX_POWER_2DBM
    SI4432_TX_POWER_5DBM
    SI4432_TX_POWER_8DBM
    SI4432_TX_POWER_11DBM
    SI4432_TX_POWER_14DBM
    SI4432_TX_POWER_17DBM
    SI4432_TX_POWER_20DBM
    */
}