#include <avr/io.h>
#include "spi.h"

//#define IS_TINY
#ifdef __AVR_ATtiny85__
    #define SPI_PORT PORTB 
    #define SPI_PIN PINB 
    #define SPI_DDR DDRB
    #define SPI_DI PORTB0
    #define SPI_DO PORTB1
    #define SPI_SCK PORTB2
#else
    #define SPI_PORT PORTB 
    #define SPI_PIN PINB 
    #define SPI_DDR DDRB
    #define SPI_DI PORTB4
    #define SPI_DO PORTB3
    #define SPI_SCK PORTB5
#endif

void inline SPI_SetData(char cData)
{
    if(cData & 0x80) { //set output pin
        SPI_PORT |= (1<<SPI_DO);
    } else {
        SPI_PORT &= ~(1<<SPI_DO);
    }
}

void inline SPI_OneClock()
{
    SPI_PORT |= (1<<SPI_SCK); //sck front
    SPI_PORT &= ~(1<<SPI_SCK); //sck front
}

void inline SPI_Init()
{
    SPI_PORT &= ~((1<<SPI_SCK)|(1<<SPI_DI));
    SPI_PORT |= (1<<SPI_DO);
    SPI_DDR &= ~(1<<SPI_DI);                //Configure DI pin as input
    SPI_DDR |= (1<<SPI_DO)|(1<<SPI_SCK);    //Configure SCK & DO pins as outputs
}

void inline SPI_Disable()
{
    SPI_DDR &= ~(1<<SPI_DI);                //Disable DI
    SPI_DDR &= ~(1<<SPI_DO)|(1<<SPI_SCK);   //Disable SCK
}

char SPI_Transmit (char cData)
{
    unsigned char rData=0,i;
    for(i=0x80;i!=0;i>>=1)
    {
        if(cData & i) { //set output pin
            SPI_PORT |= (1<<SPI_DO);
        } else {
            SPI_PORT &= ~(1<<SPI_DO);
        }
        SPI_PORT |= (1<<SPI_SCK); //sck front
        if(SPI_PIN & (1<<SPI_DI)) { //read input pin
            rData |= i;
        } else {
            rData &= ~i;
        }
        SPI_PORT &= ~(1<<SPI_SCK); //sck front
    }
    return rData;
}
