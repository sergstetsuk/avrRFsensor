/**
 * Name: main.c
 * Project: avrRFsensor
 * Author: Serg Stetsuk
 * Creation Date: 2014.10.28
 * Tabsize: 4
 * Copyright: (c) 2014 Serg Stetsuk
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * This Revision: $Id: main.c 1 2014.10.28 15:00 cs $
 */

/**
 * This example should run on most AVRs with only little changes. No special
 * hardware resources except INT0 are used. You may have to change usbconfig.h for
 * different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
 * at least be connected to INT0 as well.
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <avr/cpufunc.h>   /* for _NOP() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>
#include <avr/sleep.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"

#include "spi.h"
#include "lcd.h"
#include "rfm69hw.h"
#include "buzzer.h"

#define MODE_UNDEF 0
#define MODE_USB 1
#define MODE_TX 2
#define MODE_RX 3
#define MODE_MON 4

#define PACKET_LENGTH 16
#define EEPROM_LENGTH 512

#define TM_ONETICK 250
#define SetTimer(x) Timer1 = x/TM_ONETICK

#define TM_USBCONNECT 1000/TM_ONETICK
#define TM_NOW 0
#define TM_RETRY 2000
#define TM_WAIT_CHECK 60000
#define TM_WAIT_ALARM 10000  //was 15000
#define TM_WAIT_ALARM_LONG 60000
#define TM_FAST TM_ONETICK
#define TM_TRANSMIT 1000
#define TM_BUZZER 2000

#define MAX_CHECK_RETRY 5
char SendNextAlarm(PacketStruc* Packet);
void InitAlarm();

typedef struct __attribute__((packed)){
    uchar ID;
    uchar DebugMode;
    uchar CryptKey[16];
    uchar Monitor[30];
    } RunTimeConfigStruc;

RunTimeConfigStruc RunTimeConfig __attribute__ ((section(".eeprom")));

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x03,                    //  COLLECTION (ReportID)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0,                          //  END_COLLECTION
    0xa1, 0x03,                    //  COLLECTION (ReportID)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x96, 0x00, 0x02,              //   REPORT_COUNT (512)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0,                          //  END_COLLECTION
    0xc0                           // END_COLLECTION
};
/* The following variables store the status of the current data transfer */
static int    currentAddress;
static int    bytesRemaining;
static volatile unsigned long TickCounter,ErrTickCounter;
static uchar    WorkingMode;
static uchar    operationMode;
static uchar    options;
static uchar    debugmode;
static uchar    State,PrevState;
static uchar    minRSSIValue;
static int      Timer1, PrevTimer1, Timer2, Timer3;
static uchar    MyID;
static uchar    ForbidID;
static uchar    CheckCounter;
static uchar*   CurrentPtr;
static PacketStruc RxPacket;
static PacketStruc TxPacket;
static PacketStruc ErPacket;

#define OP_READINGPACKET 0x01
//~ #define OP_ALARMTRIGGER 0x02
#define OP_RECEIVEDELAY 0x04
#define OP_TRYADJACENT 0x08
#define OP_TRIGGERONCE 0x10

enum COMMANDS {
    CM_UNDF,
    CM_TEST,
    CM_ANSW,
    CM_ALRM,
    CM_EXEC
};

enum STATES {
    ST_UNDF,
    ST_WAIT,
    ST_CHCK,
    ST_ALRM,
    ST_RETR
};
enum INFO_OPTIONS {
    INFO_UNDF,
    INFO_RESTART,
    INFO_NOLINK,
    INFO_NOANSWER
};

#define END_LIST 0xFF

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    int shift = 0;
    if(currentAddress == 0)
        shift = 1;
    if (operationMode == 1) {
        uchar i=shift;
        if (currentAddress == 0) {
            options &= ~OP_READINGPACKET;
            if (((ReadRFM69HW(RegAutoModes) & 0x03) != AUTOMODES_INTERMEDIATE_TRANSMITTER &&
                ReadRFM69HW(RegIrqFlags1) & IRQFLAGS1_AUTOMODE)) {
                options |= OP_READINGPACKET;
            }
        }
        if (len > bytesRemaining)
            len = bytesRemaining;
        if (options & OP_READINGPACKET) {
            while (ReadRFM69HW(RegIrqFlags2) & IRQFLAGS2_FIFONOTEMPTY && i<len) {
                data[i++] = ReadRFM69HW(RegFifo);
            }
        }
        while (i<len) {
            data[i++] = 0;
        }
    } else
    if (operationMode == 2) {
        if(len > bytesRemaining)
            len = bytesRemaining;
        eeprom_read_block(data+shift, (uchar *)0 + currentAddress, len-shift);
    }
    currentAddress += len-shift;
    bytesRemaining -= len;
    //todo: maybe here we can turn on debugging information remotely
    if(debugmode == 1 && bytesRemaining <= 5 && bytesRemaining > 0) {
            //init Analog Comparator
            ADCSRB = (0<<ACME);
            ACSR = (0<<ACD)|(0<<ACBG)|(1<<ACI)|(0<<ACIE)|(0<<ACIC);
            DIDR1 = (1<<AIN1D)|(1<<AIN0D);
            //ACMPInit()

        data[len-1] = ReadRFM69HW(RegIrqFlags1); //debug
        data[len-2] = ReadRFM69HW(RegIrqFlags2); //debug
        data[len-3] = minRSSIValue;//ReadRFM69HW(RegRssiValue); //debug
        data[len-4] = ReadRFM69HW(RegOpMode); //debug
        data[len-5] = ReadRFM69HW(RegAutoModes); //debug
        data[len-6] = ACSR;
        minRSSIValue = 255;
    }
    return len;
}

/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    int shift = 0;
    if(currentAddress == 0)
        shift = 1;
    if(bytesRemaining == 0)
        return 1;
    if (operationMode == 1) { /*WRITE PACKET*/
        if(len > bytesRemaining)
            len = bytesRemaining;
        unsigned char * ptr = (unsigned char*)&TxPacket;
        ptr += currentAddress;
        int i;
        for (i=shift; i<len; i++) {
            ptr[i-shift] = data[i];
        }
        if(bytesRemaining == len) {
            InitRFM69HWtx();
            SendPacket(&TxPacket);
            Timer2 = TM_TRANSMIT/TM_ONETICK;
        }
    } else
    if (operationMode == 2) { /*WRITE EEPROM*/
        if(len > bytesRemaining)
            len = bytesRemaining;
            eeprom_write_block(data+shift, (uchar *)0 + currentAddress, len-shift);
    }
    currentAddress += len-shift;
    bytesRemaining -= len;
    return bytesRemaining == 0; /* return 1 if this was the last chunk */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
             switch(rq->wValue.bytes[0])
            {
                case 0x01:
                            bytesRemaining = PACKET_LENGTH+1;
                            currentAddress = 0;
                            operationMode = 1; /*RECV PACKET*/
                            break;
                case 0x02:
                            bytesRemaining = EEPROM_LENGTH+1;
                            currentAddress = 0;
                            operationMode = 2; /*READ EEPROM*/
                            break;
                default:
                            bytesRemaining = 0;
                            currentAddress = 0;
                            operationMode = 0; /*reset operation mode*/

            }
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            switch(rq->wValue.bytes[0])
            {
                case 0x01: /*tx packet*/
                            bytesRemaining = PACKET_LENGTH+1;
                            currentAddress = 0;
                            operationMode = 1; /*SEND PACKET*/
                            break;
                case 0x02: /*update eeprom data*/
                            bytesRemaining = EEPROM_LENGTH+1;
                            currentAddress = 0;
                            operationMode = 2; /*WRITE EEPROM*/
                            break;
                default: /*error operation - clear all temp data*/
                            bytesRemaining = 0;
                            currentAddress = 0;
                            operationMode = 0;
                            break;
            }
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

void hadAddressAssigned()
{
    WorkingMode = MODE_USB;
}

/* ------------------------------------------------------------------------- */
int main(void)
{
    uchar   i;
    minRSSIValue = 255;
    debugmode = eeprom_read_byte(&RunTimeConfig.DebugMode);
    MyID = eeprom_read_byte((const uint8_t *)&RunTimeConfig.ID);
    WorkingMode = MODE_UNDEF;  //now main mode is RX
    wdt_enable(WDTO_250MS);
    /* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */
    SPI_Init(); //init spi interface
    RFM69HW_Init(); //for rfm cs
    InitRFM69HWrx(MyID); //default mode is rx between transmissions

    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();

    WDTCSR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
    //TickCounter = 0; //not needed as in AVR all is 0, especially global and static vars
    /*USB_MODE needs regular usbPoll()*/
    sei();
    while(TickCounter<TM_USBCONNECT || WorkingMode == MODE_USB){ /* main event loop for USB_MODE*/
        usbPoll();
        WDTCSR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
    }
//NOT USB_MODE works on WDT interrupt to reduce power consumption
    USB_INTR_ENABLE &= ~(1 << USB_INTR_ENABLE_BIT);
    WorkingMode = MODE_RX;  //now main mode is RX
    SetTimer(TM_NOW);
    State = ST_ALRM;
    //~ options |= OP_ALARMTRIGGER;
    ErPacket.SrcID = MyID;
    ErPacket.Cmd = CM_ALRM;
    ErPacket.ExtraInfo[0] = ErPacket.SrcID;
    ErPacket.Options = INFO_RESTART;
    ErrTickCounter = TickCounter;
    InitAlarm();
    //~ if(!SendNextAlarm(&ErPacket)) {
        //~ SetTimer(TM_WAIT_CHECK);
        //~ State = ST_WAIT;
        //~ options &= ~ OP_ALARMTRIGGER;
    //~ }
    DDRB = 0; //disable all port B pins as outputs for saving energy
    DDRC = 0; //disable all port C pins as outputs for saving energy
    DDRD = 0; //disable all port D pins as outputs for saving energy
    PORTB = 0; //disable all port B pull-ups
    PORTC = 0; //disable all port C pull-ups
    PORTD = 0; //disable all port D pull-ups
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    for(;;)
    {
        sleep_enable();
        //debug off sleep_bod_disable();
        sei();
        sleep_cpu();
        sleep_disable();
        WDTCSR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
    }
    return 0;
}

void InitCheck()
{
    CurrentPtr = (uchar*) &RunTimeConfig.Monitor[0];
    while(eeprom_read_byte(CurrentPtr) != END_LIST)
        CurrentPtr++;
    CurrentPtr++;
    //now expected element is from checklist
}
void InitAlarm()
{
    CurrentPtr = (uchar*) &RunTimeConfig.Monitor[0];
}
char SendNextCheck()
{
    uchar id = eeprom_read_byte(CurrentPtr);
    CurrentPtr++;
    if( id == END_LIST) {
        return 0;
    }
    TxPacket.DstID = id;
    TxPacket.SrcID = MyID;
    TxPacket.Cmd = CM_TEST;
    TxPacket.ExtraInfo[0] = TxPacket.SrcID;
    TxPacket.ErrTickCounter = TickCounter;
    TxPacket.Options = 0;
    InitRFM69HWtx();
    SendPacket(&TxPacket);
    Timer2 = TM_TRANSMIT/TM_ONETICK;
    id = eeprom_read_byte(CurrentPtr);
    if( id == END_LIST) {
        return 0;
    }
    return 1;
}
//SendNextAlarm is used only in the point where alarm is generated
char SendNextAlarm(PacketStruc* Packet)
{
    uchar id = eeprom_read_byte(CurrentPtr);
    CurrentPtr++;
    if(id == END_LIST) {
        return 0;
    }
    Packet->DstID = id;
    Packet->ErrTickCounter = TickCounter - ErrTickCounter;
    InitRFM69HWtx();
    SendPacket(Packet);
    Timer2 = TM_TRANSMIT/TM_ONETICK;
    id = eeprom_read_byte(CurrentPtr);
    if(id == END_LIST) {
        return 0;
    }
    return 1;
}
//SendNextRetransmit - retransmits alarms or execs to adjacent points
//adjacent points are only from alarmlist or ErrID point for CM_EXEC
char SendNextRetransmit(PacketStruc* Packet)
{
    uchar id;
    if(options & OP_TRYADJACENT) {
        options &= ~OP_TRYADJACENT;
        //LOOK IN ALARMLIST and CHECKLIST so work twice
        int i;
        for(i = 0; i<2; i++) {
            do {
                id = eeprom_read_byte(CurrentPtr);
                CurrentPtr++;
                if(id != END_LIST && id == TxPacket.ExtraInfo[0] && TxPacket.Cmd == CM_EXEC) {
                    Packet->DstID = id;
                    InitRFM69HWtx();
                    SendPacket(Packet);
                    Timer2 = TM_TRANSMIT/TM_ONETICK;
                    return 0;
                }
            } while(id != END_LIST); //scan to end of lists
        }
        InitAlarm();
    }
    do {
        id = eeprom_read_byte(CurrentPtr);
        if(id == END_LIST) {
            return 0;   //exit at the end of alarmlist
        }
        CurrentPtr++;
    } while(id == ForbidID); //just skip forbid id
    Packet->DstID = id;
    InitRFM69HWtx();
    SendPacket(Packet);
    Timer2 = TM_TRANSMIT/TM_ONETICK;
    id = eeprom_read_byte(CurrentPtr);
    if(id == END_LIST) {
        return 0;   //exit at the end of alarmlist
    }
    return 1;
}


ISR(WDT_vect)
{
//watchdog interrupt is very cosy for use in powerdown mode
    TickCounter++;  //increment global tick counter
    SPI_Init(); //init spi interface
    LCD_Init(); //init lcd interface
    RFM69HW_Init(); //for rfm cs
    Timer1--;
    Timer2--;
    Timer3--;
    //init Analog Comparator
    ADCSRB = (0<<ACME);
    ACSR = (0<<ACD)|(0<<ACBG)|(1<<ACI)|(0<<ACIE)|(0<<ACIC);
    DIDR1 = (1<<AIN1D)|(1<<AIN0D);
    //ACMP_Init()
    //......................
    if (ReadRFM69HW(RegIrqFlags1) & IRQFLAGS1_RXREADY)  //level measuring
    {
        uchar rssi = ReadRFM69HW(RegRssiValue);
        if(minRSSIValue>rssi && rssi>0) 
        {
            minRSSIValue = rssi;
        }
    }
    if (((ReadRFM69HW(RegAutoModes) & 0x03) == AUTOMODES_INTERMEDIATE_TRANSMITTER && 
        !(ReadRFM69HW(RegIrqFlags1) & IRQFLAGS1_AUTOMODE)) || Timer2 == 0)
    {
        InitRFM69HWrx(MyID);
    }
    if (WorkingMode == MODE_RX) {
//!~ #define LINK_MASK (1<<PIND0) || (1<<PIND1) || (1<<PIND2) || (1<<PIND3) || (1<<PIND4) || (1<<PIND5) || (1<<PIND6) || (1<<PIND7)
        //~ if(PIND ^ LINK_MASK) {
        //~ if(!(PIND & (1<<PIND1)) || !(PIND & (1<<PIND2))) {
        if(ACSR&(1<<ACO)) {
                //ALARM by link
            if((State == ST_WAIT || State == ST_CHCK) && options&OP_TRIGGERONCE) {
                //~ options |= OP_ALARMTRIGGER;
                ErPacket.SrcID = MyID;
                ErPacket.Cmd = CM_ALRM;
                ErPacket.ExtraInfo[0] = ErPacket.SrcID;
                ErPacket.Options = INFO_NOLINK;
                ErrTickCounter = TickCounter;
                //~ ErPacket.MyDeltaCounter = PIND; //debug
                SetTimer(TM_NOW);
                InitAlarm();
                State = ST_ALRM;
            }
            options |= OP_TRIGGERONCE;
        } else {
            options &= ~OP_TRIGGERONCE;
        }

        if(Timer1 <= 0){
            //timeout procedure
            if(State == ST_WAIT) {
                InitCheck();
                SetTimer(TM_RETRY); //retrycheck
                Timer1 += 1000/TM_ONETICK*(MyID%4);
                State = ST_CHCK;
                CheckCounter = 0;
                if(!SendNextCheck()){ //no check queue
                    SetTimer(TM_WAIT_CHECK); //waitcorrection
                    Timer1 += 1000/TM_ONETICK*(MyID%16);
                    State = ST_WAIT;
                }
            } else
            if(State == ST_CHCK) {
                SetTimer(TM_RETRY); ///retrycheck
                Timer1 +=1000/TM_ONETICK*(MyID%4);
                if (CheckCounter++ < MAX_CHECK_RETRY) {
                    CurrentPtr--; //Send the same request
                    if(!SendNextCheck()){ //the end
                        SetTimer(TM_WAIT_CHECK); //waitcorrection
                        Timer1 += 1000/TM_ONETICK*(MyID%16);
                        State = ST_WAIT;
                    }
                } else {
                    //init alarm ErPacket
                    //~ options |= OP_ALARMTRIGGER;
                    ErPacket.SrcID = MyID;
                    ErPacket.Cmd = CM_ALRM;
                    ErPacket.ExtraInfo[0] = TxPacket.SrcID;
                    ErPacket.Options = INFO_NOANSWER;
                    ErrTickCounter = TickCounter;
                    ErPacket.ExtraInfo[1] = eeprom_read_byte(CurrentPtr);
                    SetTimer(TM_RETRY);
                    InitAlarm();
                    State = ST_ALRM;
                    if(!SendNextAlarm(&ErPacket)) { //no queue
                        SetTimer(TM_WAIT_CHECK);
                        State = ST_WAIT;
                        //~ options &= ~ OP_ALARMTRIGGER;
                    }
                }
            } else
            if(State == ST_ALRM) {
                SetTimer(TM_RETRY);
                if(!SendNextAlarm(&ErPacket)) {
                    SetTimer(TM_WAIT_ALARM);
                    if(TickCounter - ErrTickCounter > TM_WAIT_ALARM_LONG/TM_ONETICK) {
                        SetTimer(TM_WAIT_ALARM_LONG);
                    }
                    InitAlarm();
                    //~ if(!(options & OP_ALARMTRIGGER)) {
                        //~ SetTimer(TM_WAIT_CHECK);
                        //~ State = ST_WAIT;
                    //~ }
                }
            } else
            if(State == ST_RETR) {
                SetTimer(TM_RETRY);
                if(!SendNextRetransmit(&TxPacket)) {
                    Timer1 = PrevTimer1; //restore Timer1 and State -fixed
                    State = PrevState;
                }
            }
        }
        if (((ReadRFM69HW(RegAutoModes) & 0x03) != AUTOMODES_INTERMEDIATE_TRANSMITTER &&
            ReadRFM69HW(RegIrqFlags1) & IRQFLAGS1_AUTOMODE))
        {
            if(!(options & OP_RECEIVEDELAY)) {
                    options |= OP_RECEIVEDELAY;
                    SPI_Disable();
                    LCD_Disable();
                    RFM69HW_Disable();
                    return;
                }
            options &= ~OP_RECEIVEDELAY;
            //received packet processing procedure after one takt delay
            ReceivePacket(&RxPacket);
    //DEBUG LCD OPERATION ALARM AND LEVEL DISPLAY
            //if ALARM - just display for portable device //LCD mode
            if(MyID == 0xFF && RxPacket.Cmd == CM_ALRM && RxPacket.Options != INFO_NOANSWER) {
                LCD_Clear();
                if(RxPacket.ErrTickCounter < 99*60*4) {
                    //show minutes
                    int show = RxPacket.ErrTickCounter/4/60;
                    LCD_TransmitDot((show%10)&0x0F,LCD_HASH);
                    LCD_TransmitDot((show/10)&0x0F,LCD_HASH);
                } else {
                    //show overflow
                    LCD_TransmitDot(4,LCD_RAW | LCD_HASH); //minuses
                    LCD_TransmitDot(4,LCD_RAW | LCD_HASH);
                }
                //~ LCD_TransmitDot((TickCounter>>2)&0x0F,0);
                //~ LCD_TransmitDot(minRSSIValue&0x0F,LCD_HASH);
                //~ LCD_TransmitDot((minRSSIValue>>4)&0x0F,LCD_HASH);
                LCD_TransmitDot(0,LCD_RAW);
                LCD_TransmitDot(RxPacket.Options&0x0F,0);
                LCD_TransmitDot(0,LCD_RAW);
                LCD_TransmitDot((RxPacket.ExtraInfo[0]%10)&0x0F,0); //ErrID
                LCD_TransmitDot((RxPacket.ExtraInfo[0]/10%10)&0x0F,0); //ErrID
                LCD_TransmitDot((RxPacket.ExtraInfo[0]/100)&0x0F,0); //ErrID
                //~ LCD_TransmitDot(RxPacket.ExtraInfo[0]&0x0F,LCD_DOT); //ErrID
                //~ LCD_TransmitDot((RxPacket.ExtraInfo[0]>>4)&0x0F,0);
                //~ LCD_TransmitDot(RxPacket.SrcID&0x0F,LCD_DOT);
                //~ LCD_TransmitDot((RxPacket.SrcID>>4)&0x0F,0);
                //~ LCD_TransmitDot(RxPacket.DstID&0x0F,LCD_DOT);
                //~ LCD_TransmitDot((RxPacket.DstID>>4)&0x0F,0);
                minRSSIValue = 255;
                Timer3 = TM_BUZZER/TM_ONETICK;
            }
    //END DEBUG
            if(RxPacket.DstID != MyID) {
                //Not mine
                SPI_Disable();
                LCD_Disable();
                RFM69HW_Disable();
                return;
            }
            if(RxPacket.Cmd == CM_TEST) {
                //FORM ANSWER I'M PRESENT (Maybe with status)
                RxPacket.Cmd = CM_ANSW;
                RxPacket.DstID = RxPacket.SrcID;
                RxPacket.SrcID = MyID;
                RxPacket.ExtraInfo[0] = RxPacket.SrcID;
                RxPacket.ErrTickCounter = TickCounter;
                RxPacket.Options = 0;
                InitRFM69HWtx();
                SendPacket(&RxPacket);
                Timer2 = TM_TRANSMIT/TM_ONETICK;
            } else
            if(RxPacket.Cmd == CM_ANSW) {
                if(State == ST_CHCK && RxPacket.SrcID == eeprom_read_byte(CurrentPtr)) {
                    if(!SendNextCheck()){ //the end
                        SetTimer(TM_WAIT_CHECK); //waitcorrection
                        Timer1 += 1000/TM_ONETICK*(MyID%16);
                        State = ST_WAIT;
                    }
                }
            } else
            if(RxPacket.Cmd == CM_ALRM) {
                if(State != ST_RETR) {
                    PrevState = State; //save Timer1 and State
                    PrevTimer1 = Timer1;
                    State = ST_RETR;
                    SetTimer(TM_RETRY);
                    ForbidID = RxPacket.SrcID;
                    CopyPacket(&TxPacket,&RxPacket);
                    TxPacket.SrcID = RxPacket.DstID;
                    //not todo: if ErrID in checklist. Send only direct to DstID
                    //because in this case errors won't get to multidestination
                    InitAlarm();
                    if(!SendNextRetransmit(&TxPacket)) {
                        Timer1 = PrevTimer1; //restore Timer1 and State
                        State = PrevState;
                    }
                }
            } else
            if(RxPacket.Cmd == CM_EXEC) {
                if(RxPacket.ExtraInfo[0] == MyID) {
                    //~ options &= ~ OP_ALARMTRIGGER;
                    SetTimer(TM_WAIT_CHECK);
                    State = ST_WAIT;
                } else {
                    if(State != ST_RETR) {
                        //note: while in st_retr we are skipping other retransmission requests
                        PrevState = State; //save Timer1 and State
                        PrevTimer1 = Timer1;
                        State = ST_RETR;
                        SetTimer(TM_RETRY);
                        ForbidID = RxPacket.SrcID;
                        CopyPacket(&TxPacket,&RxPacket);
                        TxPacket.SrcID = RxPacket.DstID;

                        InitAlarm();
                        options |= OP_TRYADJACENT;  //trying to send only to proper destination
                        if(!SendNextRetransmit(&TxPacket)) {
                            Timer1 = PrevTimer1; //restore Timer1 and State
                            State = PrevState;
                        }
                    }
                }
            }
        }
        if(MyID == 0xFF) { //LCD mode
            if(Timer3 <= 0){
                Buzzer_Off();
                Timer3 = 0;
            } else {
                if(Timer3 & 1) {
                    Buzzer_On();
                } else {
                    Buzzer_Off();
                }
            }
        }
    }
    //Disable all output pins at the end
    SPI_Disable();
    LCD_Disable();
    RFM69HW_Disable();
}
/* ------------------------------------------------------------------------- */
