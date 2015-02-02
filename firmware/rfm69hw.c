#include <avr/io.h>
#include "rfm69hw.h"
#include "spi.h"

#ifdef __AVR_ATtiny85__
#define RFM_PORT PORTB
#define RFM_DDR DDRB
#define RFM_CS PORTB4
#else
#define RFM_PORT PORTB
#define RFM_DDR DDRB
#define RFM_CS PORTB2
#endif

void InitRFM69HW(void)
{
    RFM_PORT = (1<<RFM_CS); //CS FOR RFM69HW 1 = not selected
    RFM_DDR |= (1<<RFM_CS);  //init CS pins for devices
}

unsigned char ReadRFM69HW(unsigned char reg)
{
    unsigned char ret;
    RFM_PORT &= ~(1<<RFM_CS);  //todo: make constants RFM_PORT & RFM_CS
	SPI_Transmit(reg & 0x7F);   //todo: 0x7f is reg mask
	ret = SPI_Transmit(0);
    RFM_PORT |= (1<<RFM_CS);
    return ret;
}

unsigned char WriteRFM69HW(unsigned char reg, unsigned char val)
{
    unsigned char ret;
    RFM_PORT &= ~(1<<RFM_CS);  //todo: make constants RFM_PORT & RFM_CS
	SPI_Transmit(reg | 0x80); //todo: 0x80 is write bit
	ret = SPI_Transmit(val);
    RFM_PORT |= (1<<RFM_CS);
    return ret;
}

void InitRFM69HWCommon()
{
    unsigned char i;
    const unsigned char CONFIG[][2] =
    {
        /*0x01*/{RegOpMode,OPMODE_SEQUENCER_OFF|OPMODE_LISTEN_OFF|OPMODE_SLEEP},
        /*0x02*/{RegDataModul,DATAMODUL_PACKET_MODE|DATAMODUL_FSK|DATAMODUL_NO_SHAPING},
        /*0x03*/{RegBitRateMsb,BITRATEMSB_57600},//BitRate WAS 0x02
        /*0x04*/{RegBitRateLsb,BITRATELSB_57600},   //WAS 0x40
        /*0x05*/{RegFdevMsb,FDEVMSB_50000},  //Frequency deviation
        /*0x06*/{RegFdevLsb,FDEVLSB_50000},
        /*0x07*/{RegFrfMsb,FRFMSB_433},   //Frequency 433 MHz
        /*0x08*/{RegFrfMid,FRFMID_433},
        /*0x09*/{RegFrfLsb,FRFLSB_433},
        /*0x18*/{RegLna,LNA_ZIN_200|LNA_CURRENTGAIN|LNA_GAINSELECT_AUTO},  //LNA settings 200 Ohm
        /*0x19*/{RegRxBw,0x42}, //default
        /*0x29*/{RegRssiThresh,160}, //default gain is 0xe4=228 (-Seisitivity/2) = -114dB
        /*0x2c*/{RegPreambleMsb,0x00}, //Preamble size msb = 0 default
        /*0x2d*/{RegPreambleLsb,0x03}, //Preamble size lsb = 3 default
        /*0x2e*/{RegSyncConfig,SYNCCONGIG_SYNC_ON|SYNCCONFIG_SYNC_SIZE_2},
        /*0x2f*/{RegSyncValue1, 0x2D }, //attempt to make this compatible with sync1 byte of RFM12B lib
        /*0x30*/{RegSyncValue2, 0xE8 }, //
        /*0x37*/{RegPacketConfig1,PACKET1_FORMAT_FIXED|PACKET1_DCFREE_OFF|PACKET1_CRC_ON|PACKET1_ADDRFILTER_OFF}, //default
        /*0x38*/{RegPayloadLength,0x10},
        /*0x3C*/{RegFifoThresh,FIFOTHRESH_TXSTART_FIFONOTEMPTY|0x0F/*FIFO LEVEL VALUE*/}, //Fifo level+1 = packet size
        /*0x3d*/{RegPacketConfig2, PACKET2_AUTORXRESTART_OFF|PACKET2_AES_OFF}, //RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        /*0x6f*/{RegTestDagc, TESTDAGC_IMPROVED_LOWBETA0}, // run DAGC continuously in RX mode, recommended default for AfcLowBetaOn=0
            /*0x11*/{RegPaLevel,PALEVEL_PA0_OFF|PALEVEL_PA1_ON|PALEVEL_PA2_ON|PALEVEL_OUTPUTPOWER_11111},  //20dB
            /*0x13*/{RegOcp,OCP_OFF},  //20dB
            /*0x5a*/{RegTestPa1,TESTPA1_20dBm},  //20dB
            /*0x5c*/{RegTestPa2,TESTPA2_20dBm},  //20dB
        /*end   */ {255,0}
    };

    //~ do WriteRFM69HW(RegSyncValue1, 0xaa); while (ReadRFM69HW(RegSyncValue1) != 0xaa);
	//~ do WriteRFM69HW(RegSyncValue2, 0x55); while (ReadRFM69HW(RegSyncValue2) != 0x55);

    for (i = 0; CONFIG[i][0] != 255; i++)
        WriteRFM69HW(CONFIG[i][0], CONFIG[i][1]);
}

void InitRFM69HWrx()
{
    InitRFM69HWCommon();
    //~ WriteRFM69HW(RegAutoModes,AUTOMODES_ENTER_CRCOK
                             //~ |AUTOMODES_EXIT_FIFOEMPTY
                             //~ |AUTOMODES_INTERMEDIATE_SLEEP); //enter crcok,exit fifoempty, interstate sleep
    //WriteRFM69HW(RegOpMode,OPMODE_RX);
    WriteRFM69HW(RegOpMode,OPMODE_STNDBY|OPMODE_SEQUENCER_ON);
    WriteRFM69HW(RegRxTimeout2,/*TimeoutRxStart*/0xFF);
    /*0x0D*/WriteRFM69HW(RegListen1,LISTEN1_RESOL_IDLE_262000|LISTEN1_RESOL_RX_262000|LISTEN1_CRITERIA_RSSI|LISTEN1_END_10);  //
    /*0x0E*/WriteRFM69HW(RegListen2,/*LISTEN2_COEFIDLE*/0x0F);  //
    /*0x0F*/WriteRFM69HW(RegListen3,/*LISTEN2_COEFRX*/0x20);  //
    WriteRFM69HW(RegOpMode,OPMODE_LISTEN_ON|OPMODE_STNDBY|OPMODE_SEQUENCER_ON);
    
}

void InitRFM69HWtx()
{
    InitRFM69HWCommon();
    WriteRFM69HW(RegAutoModes,AUTOMODES_ENTER_FIFOLEVEL
                             |AUTOMODES_EXIT_PACKETSENT
                             |AUTOMODES_INTERMEDIATE_TRANSMITTER); //enter fifolevel,exit packetsent, interstate tx
}
