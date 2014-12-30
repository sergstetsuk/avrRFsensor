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
static volatile unsigned int TickCounter;
void Delay1s(void)
{
    volatile unsigned int cnt,cnt1;
    for (cnt1 = 0; cnt1 < 10; cnt1++)
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
    TickCounter = 0;
    /*debug code to indicate restart*/
    SPI_Init(); //init spi interface
    LCD_Init(); //init lcd device
    DDRB |= (1<<DDB4);  //init CS pins for devices
    PORTB = (1<<PORTB4); //CS FOR RFM69HW 1 = not selected
        LCD_Clear();
        LCD_Transmit((0x0F));
        LCD_Transmit((0x0F));
        LCD_Transmit((0x0F));
    /*end of debug code to indicate restart*/

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    for(;;)
    {
        WDTCR |= (1<<WDE) | (1<<WDIE);  //enable watchdog + enable interrupt from watchdog 
        sleep_enable();
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
    unsigned char i,r0,r1,r2; //some needed vars
    TickCounter++;  //increment global tick counter
    SPI_Init(); //init spi interface
    LCD_Init(); //init lcd device
    DDRB |= (1<<DDB4);  //init CS pins for devices
    PORTB = (1<<PORTB4); //CS FOR RFM69HW 1 = not selected

	i = TickCounter & 0x3F;	//most significant bit is write(1)/read(0) flag

#if 1
    if (i == 0x01)
    {
        InitRFM69HWtx();
    } else
    if ( !(i%4) )
    {
        WriteRFM69HW(RegFifo,0x90);
        WriteRFM69HW(RegFifo,i);
        WriteRFM69HW(RegFifo,i);
        WriteRFM69HW(RegFifo,i);
        WriteRFM69HW(RegFifo,i);
        WriteRFM69HW(RegFifo,i);
        WriteRFM69HW(RegFifo,i);
        WriteRFM69HW(RegFifo,0x70);
    }
        
#else
    if (i == 0x00)
    {
        InitRFM69HWstndby();
    }
    if (i == 0x01)
    {
        InitRFM69HWrx();
    }
    
    //~ if (ReadRFM69HW(RegIrqFlags2)&0x40)
    //~ {
        //~ c=0;
        //~ LCD_Clear();
        //~ while (ReadRFM69HW(RegIrqFlags2)&0x40)
        //~ {
            //~ c++;
            //~ r0 = ReadRFM69HW(RegFifo);
            //~ //!~ LCD_Transmit((r0 & 0x0F));
            //~ //!~ LCD_Transmit((r0 & 0xF0)>>4);
            //~ //!~ LCD_Transmit(255);
        //~ }
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
    //r0 = ReadRFM69HW(i);
    r0 = ReadRFM69HW(RegOpMode); //debug 0x01
    r1 = ReadRFM69HW(RegIrqFlags1); //debug 0x27
    r2 = ReadRFM69HW(RegIrqFlags2); //debug 0x28
    //LCD_Clear();
    LCD_Transmit((r2 & 0x0F));
    LCD_Transmit((r2 & 0xF0)>>4);
	//~ LCD_Transmit(255);
    LCD_Transmit((r1 & 0x0F));
    LCD_Transmit((r1 & 0xF0)>>4);
	//~ LCD_Transmit(255);
    LCD_Transmit((r0 & 0x0F));
    LCD_Transmit((r0 & 0xF0)>>4);
	//~ LCD_Transmit(255);
    LCD_Transmit((i & 0x0F));
    LCD_Transmit((i & 0xF0)>>4);

    //wdt_reset();
    //Delay1s();
}
/* ------------------------------------------------------------------------- */
