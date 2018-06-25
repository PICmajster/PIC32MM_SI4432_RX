/*
    File Name:        :  spi2.h
    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef _SPI2_H
#define _SPI2_H

#define SPI2_DUMMY_DATA 0x0
#define SPI2_FIFO_FILL_LIMIT 0x4

//Check to make sure that the FIFO limit does not exceed the maximum allowed limit of 8
#if (SPI2_FIFO_FILL_LIMIT > 4)

    #define SPI2_FIFO_FILL_LIMIT 4

#endif

void SPI2_Initialize (void);
uint8_t SPI2_Exchange8bit( uint8_t data );
uint16_t SPI2_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived);

typedef enum {
    SPI2_SHIFT_REGISTER_EMPTY  = 1 << 7,
    SPI2_RECEIVE_OVERFLOW = 1 << 6,
    SPI2_RECEIVE_FIFO_EMPTY = 1 << 5,
    SPI2_TRANSMIT_BUFFER_FULL = 1 << 1,
    SPI2_RECEIVE_BUFFER_FULL = 1 << 0
}SPI2_STATUS;

SPI2_STATUS SPI2_StatusGet(void);

#endif