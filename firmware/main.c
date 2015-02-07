/**
 * Name: main.c
 * Project: radio mayak
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
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>
#include <avr/sleep.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */

#include "spi.h"
#include "lcd.h"
#include "rfm69hw.h"

PROGMEM const char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};
/* The following variables store the status of the current data transfer */
static uchar    currentAddress;
static uchar    bytesRemaining;
static volatile unsigned long TickCounter;

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    uchar i;
    if(len > bytesRemaining)
        len = bytesRemaining;
    //~ eeprom_read_block(data, (uchar *)0 + currentAddress, len);
    for (i=0;i<len;i++) {
        data[i] = ReadRFM69HW((currentAddress+i) & 0x7F);
    }
    data[0] = TickCounter&0x7F;
    currentAddress += len;
    bytesRemaining -= len;
    return len;
}

/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0)
        return 1;               /* end of transfer */
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_write_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return bytesRemaining == 0; /* return 1 if this was the last chunk */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

typedef struct {
    unsigned int ID;
    unsigned char WorkMode;
    unsigned char CryptKey[16];
    unsigned int Monitor[];
    } RunTimeConfigStruc;

RunTimeConfigStruc RunTimeConfig __attribute__ ((section(".eeprom")));

int main(void)
{
    uchar   i;
    wdt_enable(WDTO_1S);
    /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
     */
    /* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */
    odDebugInit();
    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
//new prog...-->
#ifdef WDTCR //re-enable WDT for interrupt+reset
        WDTCR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#else
        WDTCSR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#endif
    //TickCounter = 0; //not needed as in AVR all is 0, especially global and static vars
    /*debug code to indicate restart*/
    SPI_Init(); //init spi interface
    LCD_Init(); //init lcd device
    InitRFM69HW(); //for rfm cs
        //~ LCD_Clear();
        //~ LCD_Transmit((0x0F));
//end new prog...<--
/*end of debug code to indicate restart*/
    sei();
    for(;;){                /* main event loop */
        //wdt_reset();
        usbPoll();
#ifdef WDTCR //re-enable WDT for interrupt+reset
        WDTCR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#else
        WDTCSR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#endif
    }
    return 0;

//new prog...-->
    wdt_enable(WDTO_1S);
    //TickCounter = 0; //not needed as in AVR all is 0, especially global and static vars
    /*debug code to indicate restart*/
    SPI_Init(); //init spi interface
    LCD_Init(); //init lcd device
    InitRFM69HW(); //for rfm cs
        LCD_Clear();
        LCD_Transmit((0x0F));
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    for(;;)
    {

        sleep_enable();
        //debug off sleep_bod_disable();
        DDRB = 0; //disable pins as outputs for saving energy
        sei();
        sleep_cpu();
        sleep_disable();
#ifdef WDTCR //re-enable WDT for interrupt+reset
        WDTCR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#else
        WDTCSR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#endif
    }
    return 0;
}

ISR(WDT_vect)
{
 //   return;
//watchdog interrupt is very cosy for use in powerdown mode
    unsigned char r0,r1,r2; //some needed vars
    TickCounter++;  //increment global tick counter
    SPI_Init(); //init spi interface
    LCD_Init(); //init lcd device
    InitRFM69HW(); //for rfm cs

#if 1
    if (TickCounter == 0x01)
    {
        InitRFM69HWtx();
    } else
    if ( !(TickCounter%4)) //every 4 seconds
    {
        WriteRFM69HW(RegFifo,0x90); //id
        WriteRFM69HW(RegFifo,0xAA);
        WriteRFM69HW(RegFifo,TickCounter & 0xFF); //working time
        WriteRFM69HW(RegFifo,(TickCounter & 0xFF00) >> 8);
        WriteRFM69HW(RegFifo,(TickCounter & 0xFF0000) >> 16);
        WriteRFM69HW(RegFifo,(TickCounter & 0xFF000000) >> 24);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x00);
        WriteRFM69HW(RegFifo,0x70);
    }
        
#else
    if (TickCounter == 0x01)
    {
        InitRFM69HWrx();
    } else
    
    if (ReadRFM69HW(RegIrqFlags2)&0x40)
    {
        unsigned long tempTick;
        unsigned char sec,min,hour,day;
        
        r0 = ReadRFM69HW(RegFifo);//id
        r1 = ReadRFM69HW(RegFifo);
        *((char*) &tempTick+0) = ReadRFM69HW(RegFifo);  //timestamp
        *((char*) &tempTick+1) = ReadRFM69HW(RegFifo);
        *((char*) &tempTick+2) = ReadRFM69HW(RegFifo);
        *((char*) &tempTick+3) = ReadRFM69HW(RegFifo);

        while (ReadRFM69HW(RegIrqFlags2)&0x40)
        {
            r0 = ReadRFM69HW(RegFifo);
        }

        sec = tempTick%60;
        tempTick /= 60;
        min = tempTick%60;
        tempTick /= 60;
        hour = tempTick%24;
        tempTick /= 24;
        day = tempTick%99;
        
        LCD_Clear();
        LCD_Transmit(sec%10);
        LCD_Transmit(sec/10);
        LCD_TransmitDot(min%10, LCD_DOT);
        LCD_Transmit(min/10);
        LCD_TransmitDot(hour%10, LCD_DOT);
        LCD_Transmit(hour/10);
        LCD_TransmitDot(day%10, LCD_DOT|LCD_HASH);
        LCD_TransmitDot(day/10, LCD_HASH);
        //~ LCD_TransmitDot(0x0F, LCD_DOT|LCD_HASH);
        //~ LCD_TransmitDot(0x0F, LCD_HASH);
        return;
    }
            //~ LCD_Transmit((c & 0x0F));
            //~ LCD_Transmit((c & 0xF0)>>4);
    //~ r0 = ReadRFM69HW(RegOpMode); //debug 0x01
    //~ r1 = ReadRFM69HW(RegIrqFlags1); //debug 0x27
    //~ r2 = ReadRFM69HW(RegIrqFlags2); //debug 0x28
        //~ Delay1s();
    //~ LCD_Transmit((r2 & 0x0F));
    //~ LCD_Transmit((r2 & 0xF0)>>4);
    //~ //!~ LCD_Transmit(255);
    //~ LCD_Transmit((r1 & 0x0F));
    //~ LCD_Transmit((r1 & 0xF0)>>4);
    //~ //!~ LCD_Transmit(255);
    //~ LCD_Transmit((r0 & 0x0F));
    //~ LCD_Transmit((r0 & 0xF0)>>4);
    //~ //!~ LCD_Transmit(255);
    //~ LCD_Transmit(255);
    //~ LCD_Transmit(255);
        //~ Delay1s();
    //~ }
#endif

    //read and display debugging info
    r0 = ReadRFM69HW(RegOpMode); //debug 0x01
    r1 = ReadRFM69HW(RegIrqFlags1); //debug 0x27
    r2 = ReadRFM69HW(RegIrqFlags2); //debug 0x28

    LCD_Transmit((r2 & 0x0F));
    LCD_Transmit((r2 & 0xF0)>>4);
    LCD_TransmitDot((r1 & 0x0F), LCD_DOT);
    LCD_Transmit((r1 & 0xF0)>>4);
    LCD_Transmit((r0 & 0x0F));
    LCD_Transmit((r0 & 0xF0)>>4);
    LCD_Transmit((TickCounter & 0x0F));
    LCD_Transmit((TickCounter & 0xF0)>>4);
    //end of debug info
}
/* ------------------------------------------------------------------------- */
