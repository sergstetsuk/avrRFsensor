#include <avr/io.h>
#include "spi.h"

//#define IS_TINY
#define SPI_PORT_NAME B
#define SPI_MOSI_BIT 3
#define SPI_MISO_BIT 4
#define SPI_SCK_BIT 5

#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SPI_MOSI PORTB3
#define SPI_MISO PORTB4
#define SPI_SCK PORTB5


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
    PORTB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2);
    DDRB &= ~(1<<DDB0);             //tiny Configure DI pin as input
    DDRB |= (1<<DDB2)|(1<<DDB1);    //tiny Configure SCK & DO pins as outputs
    USICR = USICR_DEF;  //tiny
#else
    SPI_PORT |= (1<<SPI_MISO);
    SPI_PORT &= ~((1<<SPI_MOSI)|(1<<SPI_SCK));
    SPI_DDR |= (1<<SPI_MOSI)|(1<<SPI_SCK);    //mega Configure SCK & DO pins as outputs
    SPI_DDR &= ~(1<<SPI_MISO);             //mega Configure DI pin as input
    //~ SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
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
    //~ SPI_SetData(cData);
    //~ while(!(SPSR & (1<<SPIF))); //mega
    //~ return SPDR; //mega
    unsigned char workingBit,rData;
    for(workingBit=0x80;workingBit !=0; workingBit>>=1)
    {
        if(cData & workingBit){
            SPI_PORT |= (1<<SPI_MOSI); //set mosi
        }
        else {
            SPI_PORT &= ~(1<<SPI_MOSI);
        }

        SPI_PORT |= (1<<SPI_SCK); //clock front

        if(SPI_PORT & (1<<SPI_MISO)){
            rData |= workingBit; //read miso
        }
        else {
            rData &= ~workingBit;
        }
            SPI_PORT &= ~(1<<SPI_SCK); //clock last front
    }
        return rData;
#endif
}
