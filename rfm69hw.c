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

void InitRFM69HWCommon()
{
    unsigned char i;
    const unsigned char CONFIG[][2] =
    {
        /*0x01*/{RegOpMode,OPMODE_SEQUENCER_OFF|OPMODE_LISTENOFF|OPMODE_SLEEP},
        /*0x02*/{RegDataModul,0x00},
        /*0x03*/{RegBitRateMsb,0x02},//BitRate
        /*0x04*/{RegBitRateLsb,0x40},
        /*0x05*/{RegFdevMsb,0x03},  //Frequency deviation
        /*0x06*/{RegFdevLsb,0x33},
        /*0x07*/{RegFrfMsb,0x6c},   //Frequency 433 MHz
        /*0x08*/{RegFrfMid,0x40},
        /*0x09*/{RegFrfLsb,0x00},
        /*0x18*/{RegLna,0x88},  //default 0x08 (200 Ohm), recommended 0x88 (50 Ohm)
        /*0x19*/{RegRxBw,0x42},
        /*0x29*/{RegRssiThresh,220}, //default gain is 0xe4=228 (-Seisitivity/2) = -114dB
        /*0x2c*/{RegPreambleMsb,0x00}, //Preamble size msb = 0 default
        /*0x2d*/{RegPreambleLsb,0x03}, //Preamble size lsb = 3 default
        /*0x2e*/{RegSyncConfig,0x88},
        /*0x2f*/{RegSyncValue1, 0x2d }, //attempt to make this compatible with sync1 byte of RFM12B lib
        /*0x30*/{RegSyncValue2, 0xe8 }, //
        /*0x37*/{RegPacketConfig1,0x10}, //default
        /*0x38*/{RegPayloadLength,0x08},
        /*0x3C*/{RegFifoThresh,0x87},
        /*0x3d*/{RegPacketConfig2, 0x00}, //RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        /*0x6f*/{RegTestDagc, 0x30}, // run DAGC continuously in RX mode, recommended default for AfcLowBetaOn=0
            /*0x11*/{RegPaLevel,0x7F},  //20dB
            /*0x13*/{RegOcp,0x0F},  //20dB
            /*0x5a*/{RegTestPa1,0x5D},  //20dB
            /*0x5c*/{RegTestPa2,0x7C},  //20dB
        /*end   */ {255,0}
    };

    do WriteRFM69HW(RegSyncValue1, 0xaa); while (ReadRFM69HW(RegSyncValue1) != 0xaa);
	do WriteRFM69HW(RegSyncValue2, 0x55); while (ReadRFM69HW(RegSyncValue2) != 0x55);

    for (i = 0; CONFIG[i][0] != 255; i++)
        WriteRFM69HW(CONFIG[i][0], CONFIG[i][1]);
}

void InitRFM69HWrx()
{
    InitRFM69HWCommon();
    WriteRFM69HW(RegAutoModes,0b01100100); //enter crcok,exit fifoempty, interstate sleep
    WriteRFM69HW(RegOpMode,OPMODE_RX);
}

void InitRFM69HWtx()
{
    InitRFM69HWCommon();
    WriteRFM69HW(RegAutoModes,0b01011011); //enter fifolevel,exit packetsent, interstate tx
    WriteRFM69HW(RegOpMode,OPMODE_TX);
}
