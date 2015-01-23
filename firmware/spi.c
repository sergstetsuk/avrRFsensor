#include <avr/io.h>
#include "spi.h"

//#define IS_TINY

void inline SPI_SetData(char cData)
{
#ifdef __AVR_ATtiny85__
    USIDR = cData; //tiny
#else
    SPDR = cData; //mega
#endif
}

void inline SPI_OneClock()
{
#ifdef __AVR_ATtiny85__
    USICR = USICR_DEF|(0<<USICLK);
    USICR = USICR_DEF|(1<<USICLK); //one clock
#endif
}

void inline SPI_Init()
{
#ifdef __AVR_ATtiny85__
    //!PORTB &= ~((1<<PORTB2)|(1<<PORTB1));
    DDRB &= ~(1<<DDB0);             //tiny Configure DI pin as input
    DDRB |= (1<<DDB2)|(1<<DDB1);    //tiny Configure SCK & DO pins as outputs
    USICR = USICR_DEF;  //tiny
#else
    DDRB &= ~(1<<DDB4);             //mega Configure DI pin as input
    DDRB |= (1<<DDB3)|(1<<DDB5);    //mega Configure SCK & DO pins as outputs
#endif
}

char SPI_Transmit (char cData)
{
#ifdef __AVR_ATtiny85__
    char i;
    SPI_SetData(cData);
    for(i=0;i<8;i++)
    {
        SPI_OneClock();
    }
    return USIDR;
#else
    SPI_SetData(cData);
    while(!(SPSR & (1<<SPIF))); //mega
    return SPDR; //mega
#endif
}
