/*
    File Name:        :  spi2.c
    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/

#include <xc.h>
#include "spi2.h"

typedef enum {
    SPI2_TRANSFER_MODE_32BIT  = 2,
    SPI2_TRANSFER_MODE_16BIT = 1,
    SPI2_TRANSFER_MODE_8BIT = 0
}SPI2_TRANSFER_MODE;

inline __attribute__((__always_inline__)) SPI2_TRANSFER_MODE SPI2_TransferModeGet(void);
static void SPI2_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );
static uint16_t SPI2_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData);

void SPI2_Initialize (void)
{
    unsigned int rData;
    rData=SPI2BUF; //clears the receive buffer
    // FRMERR disabled; 
    SPI2STAT = 0x0;
    // SPI2BRG 9; 
    SPI2BRG = 0x9;
    // AUDMONO disabled; AUDEN disabled; SPITUREN disabled; FRMERREN disabled; IGNROV disabled; SPISGNEXT disabled; SPIROVEN disabled; AUDMOD disabled; IGNTUR disabled; 
    SPI2CON2 = 0x0;
    // MCLKSEL PBCLK; DISSDO disabled; SRXISEL Last Word is Read; CKP Idle:Low, Active:High; FRMEN disabled; FRMSYPW One-Clock; SSEN disabled; FRMCNT 1; MSSEN disabled; MSTEN Master; MODE16 disabled; FRMPOL disabled; SMP middle; SIDL disabled; FRMSYNC disabled; CKE Idle to Active; MODE32 disabled; SPIFE Frame Sync pulse precedes; STXISEL Complete; DISSDI disabled; ON enabled; ENHBUF disabled; 
    //SPI2CON = 0x18220;
    SPI2CON = 0x18020;

}

static void SPI2_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData )
{

    while( SPI2STATbits.SPITBF == true )
    {

    }

    SPI2BUF = *((uint8_t*)pTransmitData);


    while ( SPI2STATbits.SPIRBE == true)
    {
    
    }

    *((uint8_t*)pReceiveData) = SPI2BUF;

}

static uint16_t SPI2_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData)
{

    uint16_t dataSentCount = 0;
    uint16_t dataReceivedCount = 0;
    uint8_t dummyDataReceived = 0;
    uint8_t dummyDataTransmit = (uint8_t)SPI2_DUMMY_DATA;
    uint8_t fifoMultiplier;
    uint8_t fifoLimit8Bit;
    uint16_t quotient;
    uint16_t remainder;
    uint16_t count = 0;
    uint8_t  *pSend, *pReceived;

    fifoMultiplier = 4;
    fifoLimit8Bit = ((SPI2_FIFO_FILL_LIMIT)*fifoMultiplier);
    quotient = (byteCount>>4);
    remainder = ((byteCount)&(0x000F));

    if((pTransmitData == NULL)&&(pReceiveData == NULL))
    {
        return dataSentCount;
    }
    else if (pTransmitData == NULL)
    {
        pReceived = (uint8_t*)pReceiveData;
        count = quotient;
        while (quotient--)
        {
            while ( SPI2STATbits.SPITBE != true );

            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;
            SPI2BUF = dummyDataTransmit;

            while ( SPI2STATbits.SPIRBF != true );

            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
        }

        dataSentCount = count*fifoLimit8Bit;
        dataReceivedCount = dataSentCount;

        if (remainder)
        {
            while ( SPI2STATbits.SPITBE != true );

            while ( remainder-- )
            {
                SPI2BUF = dummyDataTransmit;
            }
            dataSentCount = dataSentCount + (byteCount - dataSentCount);

            while( dataReceivedCount < byteCount )
            {
                if (SPI2STATbits.SPIRBE == false)
                {
                    *(pReceived++) = SPI2BUF;
                    dataReceivedCount++;
                }
            }
        }   
    }
    else if (pReceiveData == NULL)
    {
        pSend = (uint8_t*)pTransmitData;
        count = quotient;
        while (quotient--)
        {
            while ( SPI2STATbits.SPITBE != true );

            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);

            while ( SPI2STATbits.SPIRBF != true );

            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
            dummyDataReceived = SPI2BUF;
        }

        dataSentCount = count*fifoLimit8Bit;
        dataReceivedCount = dataSentCount;

        if (remainder)
        {
            while ( SPI2STATbits.SPITBE != true );

            while ( remainder-- )
            {
                SPI2BUF = *(pSend++);
            }
            dataSentCount = dataSentCount + (byteCount - dataSentCount);

            while( dataReceivedCount < byteCount )
            {
                if (SPI2STATbits.SPIRBE == false)
                {
                    dummyDataReceived = SPI2BUF;
                    dataReceivedCount++;
                }
            }
        }
    }
    else
    {
        pSend = (uint8_t*)pTransmitData;
        pReceived = (uint8_t*)pReceiveData;
        count = quotient;
        while (quotient--)
        {
            while ( SPI2STATbits.SPITBE != true );

            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);
            SPI2BUF = *(pSend++);

            while ( SPI2STATbits.SPIRBF != true );

            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
            *(pReceived++) = SPI2BUF;
        }

        dataSentCount = count*fifoLimit8Bit;
        dataReceivedCount = dataSentCount;

        if (remainder)
        {
            while ( SPI2STATbits.SPITBE != true );

            while ( remainder-- )
            {
                SPI2BUF = *(pSend++);
            }
            dataSentCount = dataSentCount + (byteCount - dataSentCount);

            while( dataReceivedCount < byteCount )
            {
                if (SPI2STATbits.SPIRBE == false)
                {
                    *(pReceived++) = SPI2BUF;
                    dataReceivedCount++;
                }
            }
        } 
    }
    return dataSentCount;
}

uint8_t SPI2_Exchange8bit( uint8_t data )
{
    uint8_t receiveData;
    
    SPI2_Exchange(&data, &receiveData);

    return (receiveData);
}


uint16_t SPI2_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived)
{
    return (SPI2_ExchangeBuffer(dataTransmitted, byteCount, dataReceived));
}

inline __attribute__((__always_inline__)) SPI2_TRANSFER_MODE SPI2_TransferModeGet(void)
{
    if (SPI2CONbits.MODE32 == 1)
        return SPI2_TRANSFER_MODE_32BIT;
    else if (SPI2CONbits.MODE16 == 1)
        return SPI2_TRANSFER_MODE_16BIT;
    else
        return SPI2_TRANSFER_MODE_8BIT;
}

SPI2_STATUS SPI2_StatusGet()
{
    return(SPI2STAT);
}


