#include <avr/io.h>
#include "spi.h"

void inline SPI_SetData(char cData)
{
    //debugoff USIDR = cData; //tiny
    SPDR = cData; //mega
}

void inline SPI_OneClock()
{
    //~ USICR = USICR_DEF|(0<<USICLK);
    //~ USICR = USICR_DEF|(1<<USICLK); //one clock
}

void inline SPI_Init()
{
    //!PORTB &= ~((1<<PORTB2)|(1<<PORTB1));
    //~ DDRB &= ~(1<<DDB0);             //tiny Configure DI pin as input
    //~ DDRB |= (1<<DDB2)|(1<<DDB1);    //tiny Configure SCK & DO pins as outputs
    //~ USICR = USICR_DEF;  //tiny
    DDRB &= ~(1<<DDB4);             //mega Configure DI pin as input
    DDRB |= (1<<DDB3)|(1<<DDB5);    //mega Configure SCK & DO pins as outputs
}

char SPI_Transmit (char cData)
{
    //debugoff char i;  //tiny
    SPI_SetData(cData);
    //debugoff for(i=0;i<8;i++) //tiny
    //debugoff {
        //debugoff SPI_OneClock();
    //debugoff }
    //debugoff return USIDR; //tiny
    while(!(SPSR & (1<<SPIF))); //mega
    return SPDR; //mega
}
