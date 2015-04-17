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
#define RegTestPa1 0x5A
#define RegReserved5B 0x5B
#define RegTestPa2 0x5C
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
//***********************************
// RegOpMode
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
// RegDataModul
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
// RegBitRateMsb, RegBitRateLsb
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
//***********************************
// RegFdevMsb, RegFdevLsb
#define FDEVMSB_0600 0x00
#define FDEVLSB_0600 0x0A
#define FDEVMSB_1200 0x00
#define FDEVLSB_1200 0x14
#define FDEVMSB_2000 0x00
#define FDEVLSB_2000 0x21
#define FDEVMSB_2400 0x00
#define FDEVLSB_2400 0x28
#define FDEVMSB_3800 0x00
#define FDEVLSB_3800 0x40
#define FDEVMSB_5000 0x00  // Default
#define FDEVLSB_5000 0x52  // Default
#define FDEVMSB_7500 0x00
#define FDEVLSB_7500 0x7B
#define FDEVMSB_10000 0x00
#define FDEVLSB_10000 0xA4
#define FDEVMSB_15000 0x00
#define FDEVLSB_15000 0xF6
#define FDEVMSB_20000 0x01
#define FDEVLSB_20000 0x48
#define FDEVMSB_25000 0x01
#define FDEVLSB_25000 0x9A
#define FDEVMSB_30000 0x01
#define FDEVLSB_30000 0xEC
#define FDEVMSB_35000 0x02
#define FDEVLSB_35000 0x3D
#define FDEVMSB_40000 0x02
#define FDEVLSB_40000 0x8F
#define FDEVMSB_45000 0x02
#define FDEVLSB_45000 0xE1
#define FDEVMSB_50000 0x03
#define FDEVLSB_50000 0x33
#define FDEVMSB_55000 0x03
#define FDEVLSB_55000 0x85
#define FDEVMSB_60000 0x03
#define FDEVLSB_60000 0xD7
#define FDEVMSB_65000 0x04
#define FDEVLSB_65000 0x29
#define FDEVMSB_70000 0x04
#define FDEVLSB_70000 0x7B
#define FDEVMSB_75000 0x04
#define FDEVLSB_75000 0xCD
#define FDEVMSB_80000 0x05
#define FDEVLSB_80000 0x1F
#define FDEVMSB_85000 0x05
#define FDEVLSB_85000 0x71
#define FDEVMSB_90000 0x05
#define FDEVLSB_90000 0xC3
#define FDEVMSB_95000 0x06
#define FDEVLSB_95000 0x14
#define FDEVMSB_100000 0x06
#define FDEVLSB_100000 0x66
#define FDEVMSB_110000 0x07
#define FDEVLSB_110000 0x0A
#define FDEVMSB_120000 0x07
#define FDEVLSB_120000 0xAE
#define FDEVMSB_130000 0x08
#define FDEVLSB_130000 0x52
#define FDEVMSB_140000 0x08
#define FDEVLSB_140000 0xF6
#define FDEVMSB_150000 0x09
#define FDEVLSB_150000 0x9A
#define FDEVMSB_160000 0x0A
#define FDEVLSB_160000 0x3D
#define FDEVMSB_170000 0x0A
#define FDEVLSB_170000 0xE1
#define FDEVMSB_180000 0x0B
#define FDEVLSB_180000 0x85
#define FDEVMSB_190000 0x0C
#define FDEVLSB_190000 0x29
#define FDEVMSB_200000 0x0C
#define FDEVLSB_200000 0xCD
#define FDEVMSB_210000 0x0D
#define FDEVLSB_210000 0x71
#define FDEVMSB_220000 0x0E
#define FDEVLSB_220000 0x14
#define FDEVMSB_230000 0x0E
#define FDEVLSB_230000 0xB8
#define FDEVMSB_240000 0x0F
#define FDEVLSB_240000 0x5C
#define FDEVMSB_250000 0x10
#define FDEVLSB_250000 0x00
#define FDEVMSB_260000 0x10
#define FDEVLSB_260000 0xA4
#define FDEVMSB_270000 0x11
#define FDEVLSB_270000 0x48
#define FDEVMSB_280000 0x11
#define FDEVLSB_280000 0xEC
#define FDEVMSB_290000 0x12
#define FDEVLSB_290000 0x8F
#define FDEVMSB_300000 0x13
#define FDEVLSB_300000 0x33
//***********************************
// RegFrfMsb, RegFrfMid, RegFrfLsb (MHz) - carrier frequency
// 315Mhz band
#define FRFMSB_314 0x4E
#define FRFMID_314 0x80
#define FRFLSB_314 0x00
#define FRFMSB_315 0x4E
#define FRFMID_315 0xC0
#define FRFLSB_315 0x00
#define FRFMSB_316 0x4F
#define FRFMID_316 0x00
#define FRFLSB_316 0x00
// 433mhz band
#define FRFMSB_433 0x6C
#define FRFMID_433 0x40
#define FRFLSB_433 0x00
#define FRFMSB_434 0x6C
#define FRFMID_434 0x80
#define FRFLSB_434 0x00
#define FRFMSB_435 0x6C
#define FRFMID_435 0xC0
#define FRFLSB_435 0x00
// 868Mhz band
#define FRFMSB_863 0xD7
#define FRFMID_863 0xC0
#define FRFLSB_863 0x00
#define FRFMSB_864 0xD8
#define FRFMID_864 0x00
#define FRFLSB_864 0x00
#define FRFMSB_865 0xD8
#define FRFMID_865 0x40
#define FRFLSB_865 0x00
#define FRFMSB_866 0xD8
#define FRFMID_866 0x80
#define FRFLSB_866 0x00
#define FRFMSB_867 0xD8
#define FRFMID_867 0xC0
#define FRFLSB_867 0x00
#define FRFMSB_868 0xD9
#define FRFMID_868 0x00
#define FRFLSB_868 0x00
#define FRFMSB_869 0xD9
#define FRFMID_869 0x40
#define FRFLSB_869 0x00
#define FRFMSB_870 0xD9
#define FRFMID_870 0x80
#define FRFLSB_870 0x00
// 915Mhz band
#define FRFMSB_902 0xE1
#define FRFMID_902 0x80
#define FRFLSB_902 0x00
#define FRFMSB_903 0xE1
#define FRFMID_903 0xC0
#define FRFLSB_903 0x00
#define FRFMSB_904 0xE2
#define FRFMID_904 0x00
#define FRFLSB_904 0x00
#define FRFMSB_905 0xE2
#define FRFMID_905 0x40
#define FRFLSB_905 0x00
#define FRFMSB_906 0xE2
#define FRFMID_906 0x80
#define FRFLSB_906 0x00
#define FRFMSB_907 0xE2
#define FRFMID_907 0xC0
#define FRFLSB_907 0x00
#define FRFMSB_908 0xE3
#define FRFMID_908 0x00
#define FRFLSB_908 0x00
#define FRFMSB_909 0xE3
#define FRFMID_909 0x40
#define FRFLSB_909 0x00
#define FRFMSB_910 0xE3
#define FRFMID_910 0x80
#define FRFLSB_910 0x00
#define FRFMSB_911 0xE3
#define FRFMID_911 0xC0
#define FRFLSB_911 0x00
#define FRFMSB_912 0xE4
#define FRFMID_912 0x00
#define FRFLSB_912 0x00
#define FRFMSB_913 0xE4
#define FRFMID_913 0x40
#define FRFLSB_913 0x00
#define FRFMSB_914 0xE4
#define FRFMID_914 0x80
#define FRFLSB_914 0x00
#define FRFMSB_915 0xE4  // Default
#define FRFMID_915 0xC0  // Default
#define FRFLSB_915 0x00  // Default
#define FRFMSB_916 0xE5
#define FRFMID_916 0x00
#define FRFLSB_916 0x00
#define FRFMSB_917 0xE5
#define FRFMID_917 0x40
#define FRFLSB_917 0x00
#define FRFMSB_918 0xE5
#define FRFMID_918 0x80
#define FRFLSB_918 0x00
#define FRFMSB_919 0xE5
#define FRFMID_919 0xC0
#define FRFLSB_919 0x00
#define FRFMSB_920 0xE6
#define FRFMID_920 0x00
#define FRFLSB_920 0x00
#define FRFMSB_921 0xE6
#define FRFMID_921 0x40
#define FRFLSB_921 0x00
#define FRFMSB_922 0xE6
#define FRFMID_922 0x80
#define FRFLSB_922 0x00
#define FRFMSB_923 0xE6
#define FRFMID_923 0xC0
#define FRFLSB_923 0x00
#define FRFMSB_924 0xE7
#define FRFMID_924 0x00
#define FRFLSB_924 0x00
#define FRFMSB_925 0xE7
#define FRFMID_925 0x40
#define FRFLSB_925 0x00
#define FRFMSB_926 0xE7
#define FRFMID_926 0x80
#define FRFLSB_926 0x00
#define FRFMSB_927 0xE7
#define FRFMID_927 0xC0
#define FRFLSB_927 0x00
#define FRFMSB_928 0xE8
#define FRFMID_928 0x00
#define FRFLSB_928 0x00
//***********************************
// RegOsc1
#define OSC1_RCCAL_START 0x80
#define OSC1_RCCAL_DONE 0x40
//***********************************
// RegAfcCtrl
#define AFC_STANDARD 0x00
#define AFC_IMPROVED 0x20
//***********************************
// RegListen1
#define LISTEN1_RESOL_IDLE_64 0x40
#define LISTEN1_RESOL_IDLE_4100 0x80 // Default
#define LISTEN1_RESOL_IDLE_262000 0xC0
#define LISTEN1_RESOL_RX_64 0x10 // Default
#define LISTEN1_RESOL_RX_4100 0x20
#define LISTEN1_RESOL_RX_262000 0x30
#define LISTEN1_CRITERIA_RSSI 0x00  // Default
#define LISTEN1_CRITERIA_RSSIANDSYNC 0x08
#define LISTEN1_END_00 0x00 //Chip stays in rx mode
#define LISTEN1_END_01 0x02 // Default. chip stays in Rx mode until PayloadReady or Timeout interrupt occurs. It then goes to the mode defined by Mode
#define LISTEN1_END_10 0x04 //chip stays in Rx mode until PayloadReady or Timeout interrupt occurs. FIFO content is lost at next Rx wakeup.
//***********************************
// RegListen2
#define LISTEN2_COEFIDLE 0xF5 //tListenIdle = ListenCoefIdle * ListenResolIdle
//***********************************
// RegListen3
#define LISTEN2_COEFRX 0x20 //tListenRx = ListenCoefRx * ListenResolRx
//***********************************
// RegVersion
#define VERSION_VALUE 0x24 //Version code of the chip. Bits 7-4 full revision number, 3-0 metal mask revision number
//***********************************
// RegPaLevel
#define PALEVEL_PA0_ON 0x80  // Default
#define PALEVEL_PA0_OFF 0x00
#define PALEVEL_PA1_ON 0x40
#define PALEVEL_PA1_OFF 0x00  // Default
#define PALEVEL_PA2_ON 0x20
#define PALEVEL_PA2_OFF 0x00  // Default

#define PALEVEL_OUTPUTPOWER_00000 0x00
#define PALEVEL_OUTPUTPOWER_00001 0x01
#define PALEVEL_OUTPUTPOWER_00010 0x02
#define PALEVEL_OUTPUTPOWER_00011 0x03
#define PALEVEL_OUTPUTPOWER_00100 0x04
#define PALEVEL_OUTPUTPOWER_00101 0x05
#define PALEVEL_OUTPUTPOWER_00110 0x06
#define PALEVEL_OUTPUTPOWER_00111 0x07
#define PALEVEL_OUTPUTPOWER_01000 0x08
#define PALEVEL_OUTPUTPOWER_01001 0x09
#define PALEVEL_OUTPUTPOWER_01010 0x0A
#define PALEVEL_OUTPUTPOWER_01011 0x0B
#define PALEVEL_OUTPUTPOWER_01100 0x0C
#define PALEVEL_OUTPUTPOWER_01101 0x0D
#define PALEVEL_OUTPUTPOWER_01110 0x0E
#define PALEVEL_OUTPUTPOWER_01111 0x0F
#define PALEVEL_OUTPUTPOWER_10000 0x10
#define PALEVEL_OUTPUTPOWER_10001 0x11
#define PALEVEL_OUTPUTPOWER_10010 0x12
#define PALEVEL_OUTPUTPOWER_10011 0x13
#define PALEVEL_OUTPUTPOWER_10100 0x14
#define PALEVEL_OUTPUTPOWER_10101 0x15
#define PALEVEL_OUTPUTPOWER_10110 0x16
#define PALEVEL_OUTPUTPOWER_10111 0x17
#define PALEVEL_OUTPUTPOWER_11000 0x18
#define PALEVEL_OUTPUTPOWER_11001 0x19
#define PALEVEL_OUTPUTPOWER_11010 0x1A
#define PALEVEL_OUTPUTPOWER_11011 0x1B
#define PALEVEL_OUTPUTPOWER_11100 0x1C
#define PALEVEL_OUTPUTPOWER_11101 0x1D
#define PALEVEL_OUTPUTPOWER_11110 0x1E
#define PALEVEL_OUTPUTPOWER_11111 0x1F  // Default
//***********************************
// RegPaRamp
#define PARAMP_3400 0x00
#define PARAMP_2000 0x01
#define PARAMP_1000 0x02
#define PARAMP_500 0x03
#define PARAMP_250 0x04
#define PARAMP_125 0x05
#define PARAMP_100 0x06
#define PARAMP_62 0x07
#define PARAMP_50 0x08
#define PARAMP_40 0x09  // Default
#define PARAMP_31 0x0A
#define PARAMP_25 0x0B
#define PARAMP_20 0x0C
#define PARAMP_15 0x0D
#define PARAMP_12 0x0E
#define PARAMP_10 0x0F
//***********************************
// RegOcp
#define OCP_OFF 0x0F
#define OCP_ON 0x1A  // Default

#define OCP_TRIM_45 0x00
#define OCP_TRIM_50 0x01
#define OCP_TRIM_55 0x02
#define OCP_TRIM_60 0x03
#define OCP_TRIM_65 0x04
#define OCP_TRIM_70 0x05
#define OCP_TRIM_75 0x06
#define OCP_TRIM_80 0x07
#define OCP_TRIM_85 0x08
#define OCP_TRIM_90 0x09
#define OCP_TRIM_95 0x0A  // Default
#define OCP_TRIM_100 0x0B
#define OCP_TRIM_105 0x0C
#define OCP_TRIM_110 0x0D
#define OCP_TRIM_115 0x0E
#define OCP_TRIM_120 0x0F
//***********************************
// RegLna
#define LNA_ZIN_50 0x00
#define LNA_ZIN_200 0x80  // Default

#define LNA_CURRENTGAIN 0x08

#define LNA_GAINSELECT_AUTO 0x00  // Default
#define LNA_GAINSELECT_MAX 0x01
#define LNA_GAINSELECT_MAXMINUS6 0x02
#define LNA_GAINSELECT_MAXMINUS12 0x03
#define LNA_GAINSELECT_MAXMINUS24 0x04
#define LNA_GAINSELECT_MAXMINUS36 0x05
#define LNA_GAINSELECT_MAXMINUS48 0x06
//***********************************
// RegIrqFlags1
#define IRQFLAGS1_MODEREADY 0x80
#define IRQFLAGS1_RXREADY 0x40
#define IRQFLAGS1_TXREADY 0x20
#define IRQFLAGS1_PLLLOCK 0x10
#define IRQFLAGS1_RSSI 0x08
#define IRQFLAGS1_TIMEOUT 0x04
#define IRQFLAGS1_AUTOMODE 0x02
#define IRQFLAGS1_SYNCADDRESSMATCH 0x01
//***********************************
// RegIrqFlags2
#define IRQFLAGS2_FIFOFULL 0x80
#define IRQFLAGS2_FIFONOTEMPTY 0x40
#define IRQFLAGS2_FIFOLEVEL 0x20
#define IRQFLAGS2_FIFOOVERRUN 0x10
#define IRQFLAGS2_PACKETSENT 0x08
#define IRQFLAGS2_PAYLOADREADY 0x04
#define IRQFLAGS2_CRCOK 0x02
//***********************************
// RegRssiThresh
#define RSSITHRESH_VALUE_114 228 // Default gain is 0xE4=228 (-Seisitivity/2) = -114dB
//***********************************
// RegSyncConfig
#define SYNCCONGIG_SYNC_OFF 0x00
#define SYNCCONGIG_SYNC_ON 0x80 // Default

#define SYNCCONFIG_FIFO_FILL_SYNCINTERRUPT 0x00 // Default
#define SYNCCONFIG_FIFO_FILL_MANUAL 0x40

#define SYNCCONFIG_SYNC_SIZE_1 0x00
#define SYNCCONFIG_SYNC_SIZE_2 0x08
#define SYNCCONFIG_SYNC_SIZE_3 0x10
#define SYNCCONFIG_SYNC_SIZE_4 0x18 // Default
#define SYNCCONFIG_SYNC_SIZE_5 0x20
#define SYNCCONFIG_SYNC_SIZE_6 0x28
#define SYNCCONFIG_SYNC_SIZE_7 0x30
#define SYNCCONFIG_SYNC_SIZE_8 0x38

#define SYNCCONFIG_TOL_0 0x00 // Default
#define SYNCCONFIG_TOL_1 0x01
#define SYNCCONFIG_TOL_2 0x02
#define SYNCCONFIG_TOL_3 0x03
#define SYNCCONFIG_TOL_4 0x04
#define SYNCCONFIG_TOL_5 0x05
#define SYNCCONFIG_TOL_6 0x06
#define SYNCCONFIG_TOL_7 0x07
//***********************************
// RegPacketConfig1
#define PACKET1_FORMAT_FIXED 0x00  // Default
#define PACKET1_FORMAT_VARIABLE 0x80

#define PACKET1_DCFREE_OFF 0x00  // Default
#define PACKET1_DCFREE_MANCHESTER 0x20
#define PACKET1_DCFREE_WHITENING 0x40

#define PACKET1_CRC_ON 0x10  // Default
#define PACKET1_CRC_OFF 0x00

#define PACKET1_CRCAUTOCLEAR_ON 0x00  // Default
#define PACKET1_CRCAUTOCLEAR_OFF 0x08

#define PACKET1_ADDRFILTER_OFF 0x00  // Default
#define PACKET1_ADDRFILTER_NODE 0x02
#define PACKET1_ADDRFILTER_NODEBCAST 0x04
//***********************************
// RegAutoModes
#define AUTOMODES_ENTER_OFF 0x00  // Default
#define AUTOMODES_ENTER_FIFONOTEMPTY 0x20
#define AUTOMODES_ENTER_FIFOLEVEL 0x40
#define AUTOMODES_ENTER_CRCOK 0x60
#define AUTOMODES_ENTER_PAYLOADREADY 0x80
#define AUTOMODES_ENTER_SYNCADDRMATCH 0xA0
#define AUTOMODES_ENTER_PACKETSENT 0xC0
#define AUTOMODES_ENTER_FIFOEMPTY 0xE0

#define AUTOMODES_EXIT_OFF 0x00  // Default
#define AUTOMODES_EXIT_FIFOEMPTY 0x04
#define AUTOMODES_EXIT_FIFOLEVEL 0x08
#define AUTOMODES_EXIT_CRCOK 0x0C
#define AUTOMODES_EXIT_PAYLOADREADY 0x10
#define AUTOMODES_EXIT_SYNCADDRMATCH 0x14
#define AUTOMODES_EXIT_PACKETSENT 0x18
#define AUTOMODES_EXIT_RXTIMEOUT 0x1C

#define AUTOMODES_INTERMEDIATE_SLEEP 0x00  // Default
#define AUTOMODES_INTERMEDIATE_STANDBY 0x01
#define AUTOMODES_INTERMEDIATE_RECEIVER 0x02
#define AUTOMODES_INTERMEDIATE_TRANSMITTER 0x03
//***********************************
// RegFifoThresh
#define FIFOTHRESH_TXSTART_FIFOTHRESH 0x00
#define FIFOTHRESH_TXSTART_FIFONOTEMPTY 0x80  // Default
//***********************************
// RegPacketConfig2
#define PACKET2_RXRESTARTDELAY_1BIT 0x00  // Default
#define PACKET2_RXRESTARTDELAY_2BITS 0x10
#define PACKET2_RXRESTARTDELAY_4BITS 0x20
#define PACKET2_RXRESTARTDELAY_8BITS 0x30
#define PACKET2_RXRESTARTDELAY_16BITS 0x40
#define PACKET2_RXRESTARTDELAY_32BITS 0x50
#define PACKET2_RXRESTARTDELAY_64BITS 0x60
#define PACKET2_RXRESTARTDELAY_128BITS 0x70
#define PACKET2_RXRESTARTDELAY_256BITS 0x80
#define PACKET2_RXRESTARTDELAY_512BITS 0x90
#define PACKET2_RXRESTARTDELAY_1024BITS 0xA0
#define PACKET2_RXRESTARTDELAY_2048BITS 0xB0
#define PACKET2_RXRESTARTDELAY_NONE 0xC0
#define PACKET2_RXRESTART 0x04

#define PACKET2_AUTORXRESTART_ON 0x02  // Default
#define PACKET2_AUTORXRESTART_OFF 0x00

#define PACKET2_AES_ON 0x01
#define PACKET2_AES_OFF 0x00  // Default
//***********************************
// RegTemp1
#define TEMP1_MEAS_START 0x08
#define TEMP1_MEAS_RUNNING 0x04
//***********************************
// RegTestLna
#define TESTLNA_SENSITIVITY_HIGH 0x2D
#define TESTLNA_SENSITIVITY_NORMAL 0x1B
//***********************************
// RegTestPa1
#define TESTPA1_NORMAL 0x55
#define TESTPA1_20dBm 0x5D
//***********************************
// RegTestPa2
#define TESTPA2_NORMAL 0x70
#define TESTPA2_20dBm 0x7C
//***********************************
// RegTestDagc 0x6F: demodulator config and IO mode config
#define TESTDAGC_NORMAL 0x00  // Reset value
#define TESTDAGC_IMPROVED_LOWBETA1 0x20  //
#define TESTDAGC_IMPROVED_LOWBETA0 0x30  // Recommended default
//***********************************
//***********************************
typedef struct {
    unsigned short DstID;
    unsigned short SrcID;
    unsigned char Cmd;
    unsigned char Options;
    unsigned short ErrID;
    unsigned long ErrTickCounter;
    unsigned short ExtraInfo;
    unsigned short DebugInfo;
    } PacketStruc;

//***********************************

unsigned char ReadRFM69HW(unsigned char);
unsigned char WriteRFM69HW(unsigned char, unsigned char);
void InitRFM69HWstndby();
void InitRFM69HWsleep();
void InitRFM69HWrx(unsigned char);
void InitRFM69HWrxusb();
void InitRFM69HWtx();
void InitRFM69HW();
void SendPacket (PacketStruc *);
void ReceivePacket (PacketStruc *);
#endif /*RFM69HW_H*/