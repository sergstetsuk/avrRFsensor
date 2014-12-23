#include <avr/io.h>
#include "spi.h"

void inline SPI_SetData(char cData)
{
    USIDR = cData;
}

void inline SPI_OneClock()
{
    USICR = USICR_DEF|(0<<USICLK);
    USICR = USICR_DEF|(1<<USICLK); //one clock
}

void inline SPI_Init()
{
    DDRB &= ~(1<<DDB0);             //Configure DI pin as input
    DDRB |= (1<<DDB2)|(1<<DDB1);    //Configure SCK & DO pins as outputs
    //PORTB &= ~((1<<PORTB2)|(1<<PORTB1));
    USICR = USICR_DEF;
}

char SPI_Transmit (char cData)
{
    char i;
    SPI_SetData(cData);
    for(i=0;i<8;i++)
    {
        SPI_OneClock();
    }
    return USIDR;
}
