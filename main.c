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

#include "spi.h"
#include "lcd.h"

void Delay1s(void)
{
    volatile unsigned int cnt,cnt1;
    for (cnt1 = 0; cnt1 < 20; cnt1++)
    {
        wdt_reset();
        for (cnt = 0; cnt < 55555; cnt++); //~1sec Delay on a 1MHz clock
    }
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
    SPI_Init();
    LCD_Init(); 
    DDRB |= (1<<DDB4);
    PORTB = (1<<PORTB4); //CS FOR RFM69HW 1 = not selected
//    sei();
    unsigned char i,r;

    for(i=0;;i++){                /* main event loop */
	i &= 0x7F;	//most significant is write/read bit = 1/0
        wdt_reset();
    PORTB &= ~(1<<PORTB4);
	SPI_Transmit(i);
	r = SPI_Transmit(0);
    PORTB |= (1<<PORTB4);
	LCD_Clear();
        LCD_Transmit((r & 0x0F));
        LCD_Transmit((r & 0xF0)>>4);
	LCD_Transmit(255);
        LCD_Transmit((i & 0x0F));
        LCD_Transmit((i & 0xF0)>>4);

        Delay1s();
        }
    return 0;
}


/* ------------------------------------------------------------------------- */
