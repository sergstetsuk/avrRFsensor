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
    RFM_PORT |= (1<<RFM_CS); //CS FOR RFM69HW 1 = not selected
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
        /*0x01*/{RegOpMode,OPMODE_SEQUENCER_ON|OPMODE_LISTEN_OFF|OPMODE_LISTEN_ABORT|OPMODE_SLEEP},
        /*0x01*/{RegOpMode,OPMODE_SEQUENCER_ON|OPMODE_LISTEN_OFF|OPMODE_SLEEP},
        /*0x02*/{RegDataModul,DATAMODUL_PACKET_MODE|DATAMODUL_FSK|DATAMODUL_NO_SHAPING},
        /*0x03*/{RegBitRateMsb,BITRATEMSB_1200},//BitRate WAS 0x02
        /*0x04*/{RegBitRateLsb,BITRATELSB_1200},   //WAS 0x40
        /*0x05*/{RegFdevMsb,FDEVMSB_10000},  //Frequency deviation
        /*0x06*/{RegFdevLsb,FDEVLSB_10000},
        /*0x07*/{RegFrfMsb,FRFMSB_433},   //Frequency 433 MHz
        /*0x08*/{RegFrfMid,FRFMID_433},
        /*0x09*/{RegFrfLsb,FRFLSB_433},
        //~ /*0x0B*/{RegAfcCtrl,AFC_IMPROVED}, //low beta on = 1 for low modulation systems
        /*0x0B*/{RegAfcCtrl,AFC_STANDARD}, //low beta on = 0 for non-low modulation systems
        /*0x18*/{RegLna,LNA_ZIN_50/*|LNA_CURRENTGAIN*/|LNA_GAINSELECT_MAX},  //LNA settings 200 Ohm
        /*0x19*/{RegRxBw,0x4A}, //default 0x55 3.1kHz 4F
        /*0x19*/{RegAfcBw,0x45}, //default 0x55 5.2kHz 56 for auto freq correction
        /*0x1E*/{RegAfcFei,0x0C}, //autoAFC on RX start on
        //~ /*0x29*/{RegRssiThresh,228}, //default gain is 0xe4=228 (-Sensitivity/2) = -114dB
        //~ /*0x2c*/{RegPreambleMsb,0x00}, //Preamble size msb = 0 default
        //~ /*0x2d*/{RegPreambleLsb,0x03}, //Preamble size lsb = 3 default
        /*0x2e*/{RegSyncConfig,SYNCCONGIG_SYNC_ON|SYNCCONFIG_SYNC_SIZE_2},
        /*0x2f*/{RegSyncValue1, 0x2D }, //attempt to make this compatible with sync1 byte of RFM12B lib
        /*0x30*/{RegSyncValue2, 0xE8 }, //
        /*0x37*/{RegPacketConfig1,PACKET1_FORMAT_FIXED|PACKET1_DCFREE_MANCHESTER|PACKET1_CRC_ON|PACKET1_ADDRFILTER_NODE}, //default
        //~ /*0x37*/{RegPacketConfig1,PACKET1_FORMAT_FIXED|PACKET1_DCFREE_MANCHESTER|PACKET1_CRC_ON|PACKET1_ADDRFILTER_OFF}, //default
        /*0x38*/{RegPayloadLength,sizeof(PacketStruc)},
        /*0x3C*/{RegFifoThresh,FIFOTHRESH_TXSTART_FIFONOTEMPTY|(sizeof(PacketStruc)-1)/*FIFO LEVEL VALUE*/}, //Fifo level+1 = packet size
        //~ /*0x3d*/{RegPacketConfig2, PACKET2_AUTORXRESTART_OFF|PACKET2_AES_OFF}, //RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        //~ /*0x6f*/{RegTestDagc, TESTDAGC_IMPROVED_LOWBETA1}, // run DAGC continuously in RX mode, recommended AfcLowBetaOn=1 for low modulation systems
        //~ /*0x6f*/{RegTestDagc, TESTDAGC_IMPROVED_LOWBETA0}, // run DAGC continuously in RX mode, recommended AfcLowBetaOn=1 for low modulation systems
        /*0x6f*/{RegTestDagc, TESTDAGC_NORMAL}, // run DAGC continuously in RX mode, recommended AfcLowBetaOn=1 for low modulation systems
        /*end   */ {255,0}
    };

    //~ do WriteRFM69HW(RegSyncValue1, 0xaa); while (ReadRFM69HW(RegSyncValue1) != 0xaa);
	//~ do WriteRFM69HW(RegSyncValue2, 0x55); while (ReadRFM69HW(RegSyncValue2) != 0x55);

    for (i = 0; CONFIG[i][0] != 255; i++)
        WriteRFM69HW(CONFIG[i][0], CONFIG[i][1]);
}

void InitRFM69HWsleep()
{
    InitRFM69HWCommon();
    WriteRFM69HW(RegOpMode,OPMODE_SLEEP);
}

void InitRFM69HWstndby()
{
    InitRFM69HWCommon();
    WriteRFM69HW(RegOpMode,OPMODE_STNDBY);
}

void InitRFM69HWrxusb()
{
    //~ WriteRFM69HW(RegAutoModes,AUTOMODES_ENTER_CRCOK
                             //~ |AUTOMODES_EXIT_FIFOEMPTY
                             //~ |AUTOMODES_INTERMEDIATE_SLEEP); //enter crcok,exit fifoempty, interstate sleep
    InitRFM69HWCommon();
    WriteRFM69HW(RegOpMode,OPMODE_RX|OPMODE_SEQUENCER_ON);
}

void InitRFM69HWrx(unsigned char nodeAddr)
{
    InitRFM69HWCommon();
            /*0x11*/WriteRFM69HW(RegPaLevel,PALEVEL_PA0_ON|PALEVEL_PA1_OFF|PALEVEL_PA2_OFF|PALEVEL_OUTPUTPOWER_11111);  //Rx
            /*0x13*/WriteRFM69HW(RegOcp,OCP_ON);  //Rx
            /*0x5a*/WriteRFM69HW(RegTestPa1,TESTPA1_NORMAL);  //Rx
            /*0x5c*/WriteRFM69HW(RegTestPa2,TESTPA2_NORMAL);  //Rx
            /*0x58*/WriteRFM69HW(RegTestLna,TESTLNA_SENSITIVITY_NORMAL);  //HIGH = -120 dB Rx
    //~ WriteRFM69HW(RegFrfLsb,FRFLSB_433 + 0x08); //10% thumbup
    WriteRFM69HW(RegNodeAdrs,nodeAddr); //default gain is 0xe4=228 (-Sensitivity/2) = -114dB
    WriteRFM69HW(RegRssiThresh,192); //default gain is 0xe4=228 (-Sensitivity/2) = -114dB
    WriteRFM69HW(RegAutoModes,AUTOMODES_ENTER_CRCOK
                             |AUTOMODES_EXIT_FIFOEMPTY
                             |AUTOMODES_INTERMEDIATE_SLEEP); //enter crcok,exit fifoempty, interstate sleep
    /*0x0D*/WriteRFM69HW(RegListen1,LISTEN1_RESOL_IDLE_4100|LISTEN1_RESOL_RX_4100|LISTEN1_CRITERIA_RSSI|LISTEN1_END_10);  //
    /*0x0E*/WriteRFM69HW(RegListen2,/*LISTEN2_COEFIDLE*/0x38);  //
    /*0x0F*/WriteRFM69HW(RegListen3,/*LISTEN2_COEFRX*/0x02);  //
    WriteRFM69HW(RegRxTimeout2,40); //(39pream++2sync+16data+2crc+1)/2 = 30
    WriteRFM69HW(RegOpMode,OPMODE_LISTEN_ON|OPMODE_STNDBY|OPMODE_SEQUENCER_ON);
    //~ WriteRFM69HW(RegOpMode,OPMODE_RX|OPMODE_SEQUENCER_ON);
    //todo: try to make sleep instead of stndby
}

void InitRFM69HWtx()
{
    InitRFM69HWCommon();
    /*HIGH POWER SETTINGS ARE USED ONLY IN TX MODE*/
            /*0x11*/WriteRFM69HW(RegPaLevel,PALEVEL_PA0_OFF|PALEVEL_PA1_ON|PALEVEL_PA2_ON|PALEVEL_OUTPUTPOWER_11111);  //20dB Tx
            /*0x13*/WriteRFM69HW(RegOcp,OCP_OFF);  //20dB Tx
            /*0x5a*/WriteRFM69HW(RegTestPa1,TESTPA1_20dBm);  //20dB Tx
            /*0x5c*/WriteRFM69HW(RegTestPa2,TESTPA2_20dBm);  //20dB Tx

    //~ WriteRFM69HW(RegPreambleMsb,0x00); //Preamble size msb = 0 default
    WriteRFM69HW(RegPreambleLsb,40); //Preamble size must be enough for listening mode period

    WriteRFM69HW(RegAutoModes,AUTOMODES_ENTER_FIFOLEVEL
                             |AUTOMODES_EXIT_PACKETSENT
                             |AUTOMODES_INTERMEDIATE_TRANSMITTER); //enter fifolevel,exit packetsent, interstate tx
}

void CopyPacket (PacketStruc* dst,PacketStruc* src) {
    int i;
    unsigned char* dptr = (unsigned char*) dst;
    unsigned char* sptr = (unsigned char*) src;
    for(i = 0; i < sizeof(PacketStruc); i++){
        *dptr++ = *sptr++;
    }
}

void SendPacket (PacketStruc * buffer) {
    InitRFM69HWtx();
    int i;
    unsigned char * ptr = (unsigned char*) buffer;
    for(i = 0; i < sizeof(PacketStruc); i++){
        WriteRFM69HW(RegFifo,*ptr++);
    }
}

void ReceivePacket (PacketStruc * buffer) {
    int i;
    unsigned char databyte;
    unsigned char * ptr = (unsigned char*) buffer;
    for (i = 0; ReadRFM69HW(RegIrqFlags2) & IRQFLAGS2_FIFONOTEMPTY; i++) {
        databyte = ReadRFM69HW(RegFifo);
        if(i < sizeof(PacketStruc)) {
            *ptr++ = databyte;
        }
    }
}   