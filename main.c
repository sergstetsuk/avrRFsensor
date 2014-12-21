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

/*
This example should run on most AVRs with only little changes. No special
hardware resources except INT0 are used. You may have to change usbconfig.h for
different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
at least be connected to INT0 as well.
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#include "lcd.h"

void Delay1s(void)
{
volatile unsigned int cnt;
for (cnt = 0; cnt < 55555; cnt++); //~1sec Delay on a 1MHz clock
}

char SPI_Transmit (char cData)
{
    USIDR = cData;
    while(!(USISR & (1<<USISIF)));
    return USIDR;
}

int main(void)
{
    wdt_enable(WDTO_1S);
    /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
     */
    /* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */
     DDRB = (1<<DDB3)|(1<<DDB2)|(1<<DDB0);
     //USICR = (1<<USISIE)|(0<<USIOIE)|(0<<USIWM1)|(1<<USIWM0)
       //     |(0<<USICS1)|(0<<USICS0)|(0<<USICLK)|(0<<USITC);

     PORTB = 0x00;
//    sei();
    char i;
    for(i=0;;i++){                /* main event loop */
        wdt_reset();
        //PORTB ^= (1<<PORTB2); //Clock
        Delay1s();
        Delay1s();
        //if (i%18) PORTB ^= (1<<PORTB0); //Data
        //else {
        //     PORTB ^= (1<<PORTB3);  //Load data
        //     PORTB ^= (1<<PORTB3);
        //}
        LCD_Transmit(15-(i & 0x0F));
    }
    return 0;
}


/* ------------------------------------------------------------------------- */
