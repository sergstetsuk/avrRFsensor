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

#include "spi.h"
#include "lcd.h"
#include "rfm69hw.h"
static volatile unsigned long TickCounter;

typedef struct {
    unsigned int ID;
    unsigned char WorkMode;
    unsigned char CryptKey[16];
    unsigned int Monitor[];
    } RunTimeConfigStruc;

RunTimeConfigStruc RunTimeConfig __attribute__ ((section(".eeprom")));

int main(void)
{
    wdt_enable(WDTO_1S);
    //TickCounter = 0; //not needed as in AVR all is 0, especially global and static vars
    /*debug code to indicate restart*/
    SPI_Init(); //init spi interface
    LCD_Init(); //init lcd device
    PORTB = (1<<PORTB4); //CS FOR RFM69HW 1 = not selected
    DDRB |= (1<<DDB4);  //init CS pins for devices
        LCD_Clear();
        LCD_Transmit((0x0F));
    /*end of debug code to indicate restart*/

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    for(;;)
    {

#ifdef WDTCR //re-enable WDT for interrupt+reset
        WDTCR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#else
        WDTCSR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt on watchdog
#endif
        sleep_enable();
        //debug off sleep_bod_disable();
        DDRB = 0; //disable pins as outputs for saving energy
        sei();
        sleep_cpu();
        sleep_disable();
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
    DDRB |= (1<<DDB4);  //init CS pins for devices
    PORTB = (1<<PORTB4); //CS FOR RFM69HW 1 = not selected

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
