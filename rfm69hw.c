#include <avr/io.h>
#include "rfm69hw.h"
#include "spi.h"

unsigned char ReadRFM69HW(unsigned char reg)
{
    unsigned char ret;
    PORTB &= ~(1<<PORTB4);  //todo: make constants RFM_PORT & RFM_CS
	SPI_Transmit(reg & 0x7F);   //todo: 0x7f is reg mask
	ret = SPI_Transmit(0);
    PORTB |= (1<<PORTB4);
    return ret;
}

unsigned char WriteRFM69HW(unsigned char reg, unsigned char val)
{
    unsigned char ret;
    PORTB &= ~(1<<PORTB4);  //todo: make constants RFM_PORT & RFM_CS
	SPI_Transmit(reg | 0x80); //todo: 0x80 is write bit
	ret = SPI_Transmit(val);
    PORTB |= (1<<PORTB4);
    return ret;
}

void InitRFM69HWstndby()
{
    WriteRFM69HW(RegOpMode,RegOp_ModeStndby);
}

void InitRFM69HWsleep()
{
    WriteRFM69HW(RegOpMode,RegOp_ModeSleep);
}

void InitRFM69HWrx()
{
    WriteRFM69HW(RegFrfMsb,0x6c);
    WriteRFM69HW(RegFrfMid,0x4f);
    WriteRFM69HW(RegFrfLsb,0xf8);
    WriteRFM69HW(RegRssiThresh,0xde);
    WriteRFM69HW(RegSyncConfig,0b10111000);
    WriteRFM69HW(RegPacketConfig1,0b0011000);
    WriteRFM69HW(RegPayloadLength,0x08);
    WriteRFM69HW(RegFifoThresh,0x87);
    WriteRFM69HW(RegAutoModes,0b01100100); //enter crcok,exit fifoempty, interstate sleep
    WriteRFM69HW(RegOpMode,RegOp_ModeRX);
}

void InitRFM69HWtx()
{
    WriteRFM69HW(RegFrfMsb,0x6c);
    WriteRFM69HW(RegFrfMid,0x4f);
    WriteRFM69HW(RegFrfLsb,0xf8);
    WriteRFM69HW(RegSyncConfig,0b10111000);
    WriteRFM69HW(RegPacketConfig1,0b0011000);
    WriteRFM69HW(RegPayloadLength,0x08);
    WriteRFM69HW(RegFifoThresh,0x87);
    WriteRFM69HW(RegAutoModes,0b01011011); //enter fifolevel,exit packetsent, interstate tx
    WriteRFM69HW(RegOpMode,RegOp_ModeSleep);
}
