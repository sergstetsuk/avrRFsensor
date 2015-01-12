#ifndef RFM69HW_H
#define RFM69HW_H

/** Registers of RFM69HW radion communication module
 */

#define RegFifo 0x00 //FIFO data input/output
#define RegOpMode 0x01 //Operation mode register
#define RegDataModul 0x02
#define RegBitRateMsb 0x03
#define RegBitRateLsb 0x04
#define RegFdevMsb 0x05
#define RegFdevLsb 0x06
#define RegFrfMsb 0x07
#define RegFrfMid 0x08
#define RegFrfLsb 0x09
#define RegOsc1 0x0A
#define RegAfcCtrl 0x0B
#define RegReserved0C 0x0C
#define RegListen1 0x0D
#define RegListen2 0x0E
#define RegListen3 0x0F

#define RegVersion 0x10
#define RegPaLevel 0x11
#define RegPaRamp 0x12
#define RegOcp 0x13
#define RegReserved14 0x14
#define RegReserved15 0x15
#define RegReserved16 0x16
#define RegReserved17 0x17
#define RegLna 0x18
#define RegRxBw 0x19
#define RegAfcBw 0x1A
#define RegOokPeak 0x1B
#define RegOokAvg 0x1C
#define RegOokFix 0x1D
#define RegAfcFei 0x1E
#define RegAfcMsb 0x1F

#define RegAfcLsb 0x20
#define RegFeiMsb 0x21
#define RegFeiLsb 0x22
#define RegRssiConfig 0x23
#define RegRssiValue 0x24
#define RegDioMapping1 0x25
#define RegDioMapping2 0x26
#define RegIrqFlags1 0x27
#define RegIrqFlags2 0x28
#define RegRssiThresh 0x29
#define RegRxTimeout1 0x2A
#define RegRxTimeout2 0x2B
#define RegPreambleMsb 0x2C
#define RegPreambleLsb 0x2D
#define RegSyncConfig 0x2E
#define RegSyncValue1 0x2F

#define RegSyncValue2 0x30
#define RegSyncValue3 0x31
#define RegSyncValue4 0x32
#define RegSyncValue5 0x33
#define RegSyncValue6 0x34
#define RegSyncValue7 0x35
#define RegSyncValue8 0x36
#define RegPacketConfig1 0x37
#define RegPayloadLength 0x38
#define RegNodeAdrs 0x39
#define RegBroadcastAdrs 0x3A
#define RegAutoModes 0x3B
#define RegFifoThresh 0x3C
#define RegPacketConfig2 0x3D
#define RegAesKey1 0x3E
#define RegAesKey2 0x3F

#define RegAesKey3 0x40
#define RegAesKey4 0x41
#define RegAesKey5 0x42
#define RegAesKey6 0x43
#define RegAesKey7 0x44
#define RegAesKey8 0x45
#define RegAesKey9 0x46
#define RegAesKey10 0x47
#define RegAesKey11 0x48
#define RegAesKey12 0x49
#define RegAesKey13 0x4A
#define RegAesKey14 0x4B
#define RegAesKey15 0x4C
#define RegAesKey16 0x4D
#define RegTemp1 0x4E
#define RegTemp2 0x4F

#define RegReserved50 0x50
#define RegReserved51 0x51
#define RegReserved52 0x52
#define RegReserved53 0x53
#define RegReserved54 0x54
#define RegReserved55 0x55
#define RegReserved56 0x56
#define RegReserved57 0x57
#define RegTestLna 0x58
#define RegReserved59 0x59
#define RegTestPa2 0x5A
#define RegReserved5B 0x5B
#define RegTestPa1 0x5C
#define RegReserved5D 0x5D
#define RegReserved5E 0x5E
#define RegReserved5F 0x5F


#define RegReserved60 0x60
#define RegReserved61 0x61
#define RegReserved62 0x62
#define RegReserved63 0x63
#define RegReserved64 0x64
#define RegReserved65 0x65
#define RegReserved66 0x66
#define RegReserved67 0x67
#define RegReserved68 0x68
#define RegReserved69 0x69
#define RegReserved6A 0x6A
#define RegReserved6B 0x6B
#define RegReserved6C 0x6C
#define RegReserved6D 0x6D
#define RegReserved6E 0x6E
#define RegTestDagc 0x6F

#define RegReserved70 0x70
#define RegTestAfc 0x71
#define RegReserved72 0x72
#define RegReserved73 0x73
#define RegReserved74 0x74
#define RegReserved75 0x75
#define RegReserved76 0x76
#define RegReserved77 0x77
#define RegReserved78 0x78
#define RegReserved79 0x79
#define RegReserved7A 0x7A
#define RegReserved7B 0x7B
#define RegReserved7C 0x7C
#define RegReserved7D 0x7D
#define RegReserved7E 0x7E
#define RegReserved7F 0x7F

/* Configuration registers' modes
 *
 */
#define OPMODE_SEQUENCER_ON 0x00 //
#define OPMODE_SEQUENCER_OFF 0x80 //
#define OPMODE_LISTEN_ON 0x40 //Enables listen mode in standby mode
#define OPMODE_LISTEN_OFF 0x00 //Enables listen mode in standby mode
#define OPMODE_LISTEN_ABORT 0x20 //
#define OPMODE_MODE_MASK 0x1C //Mask for operation mode
#define OPMODE_SLEEP 0x00 //Sleep mode
#define OPMODE_STNDBY 0x04 //Standby mode
#define OPMODE_FS 0x08 //Frequency Synthesizer mode
#define OPMODE_TX 0x0C //Transmit mode
#define OPMODE_RX 0x10 //Receive mode
//***********************************
#define DATAMODUL_PACKET_MODE 0x00 //
#define DATAMODUL_CONTINUOUS_SYNC_MODE 0x40 //
#define DATAMODUL_CONTINUOUS_NOSYNC_MODE 0x60 //
#define DATAMODUL_FSK 0x00 //
#define DATAMODUL_OOK 0x08 //
#define DATAMODUL_NO_SHAPING 0x00 //
#define DATAMODUL_FSK_GAUS_BT10 0x01 //
#define DATAMODUL_FSK_GAUS_BT05 0x02 //
#define DATAMODUL_FSK_GAUS_BT03 0x03 //
#define DATAMODUL_OOK_BR 0x01 //
#define DATAMODUL_OOK_2BR 0x02 //
//***********************************
#define BITRATEMSB_1200 0x68
#define BITRATELSB_1200 0x2B
#define BITRATEMSB_2400 0x34
#define BITRATELSB_2400 0x15
#define BITRATEMSB_4800 0x1A
#define BITRATELSB_4800 0x0B
#define BITRATEMSB_9600 0x0D
#define BITRATELSB_9600 0x05
#define BITRATEMSB_19200 0x06
#define BITRATELSB_19200 0x83
#define BITRATEMSB_38400 0x03
#define BITRATELSB_38400 0x41
#define BITRATEMSB_76800 0x01
#define BITRATELSB_76800 0xA1
#define BITRATEMSB_153600 0x00
#define BITRATELSB_153600 0xD0

#define BITRATEMSB_57600 0x02
#define BITRATELSB_57600 0x2C
#define BITRATEMSB_115200 0x01
#define BITRATELSB_115200 0x16

unsigned char ReadRFM69HW(unsigned char);
unsigned char WriteRFM69HW(unsigned char, unsigned char);
//~ void InitRFM69HWstndby();
//~ void InitRFM69HWsleep();
void InitRFM69HWrx();
void InitRFM69HWtx();

#endif /*RFM69HW_H*/