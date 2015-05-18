#include <avr/io.h>
#include <avr/pgmspace.h>

#include "spi.h"
#include "lcd.h"
#ifdef __AVR_ATtiny85__
    #define LCD_PORT PORTC
    #define LCD_DDR DDRC
    #define LCD_LOAD PORTC4
#else
    #define LCD_PORT PORTC
    #define LCD_DDR DDRC
    #define LCD_LOAD PORTC4
#endif

const unsigned char CHARTABLE[] PROGMEM = {
      0b01111011, //0
      0b01100000, //1
      0b00110111, //2
      0b01110101, //3
      0b01101100, //4
      0b01011101, //5
      0b01011111, //6
      0b01110000, //7
      0b01111111, //8
      0b01111101, //9
      0b01111110, //A
      0b01001111, //B
      0b00011011, //C
      0b01100111, //D
      0b00011111, //E
      0b00011110, //F
                      };

void inline LCD_Init()
{
    LCD_PORT &= ~(1<<LCD_LOAD);
    LCD_DDR |= (1<<LCD_LOAD);   //Configure LOAD pin as output
}

void inline LCD_Disable()
{
    LCD_DDR &= ~(1<<LCD_LOAD);   //Configure LOAD pin as output
}

void inline LCD_Load()
{
    LCD_PORT |= (1<<LCD_LOAD);  //Load data strobe
    LCD_PORT &= ~(1<<LCD_LOAD);
}
void LCD_TransmitDot(char cData, char isdot)
{
    char tData = cData; //raw data by defaule
    if (!(isdot & LCD_RAW) && cData < sizeof(CHARTABLE)/sizeof(CHARTABLE[0]))
    {
        tData = pgm_read_byte(&CHARTABLE[(int)cData]);
    }
    if( isdot & LCD_DOT){
        SPI_SetData(0x80);
    } else {
        SPI_SetData(0x00);
    }
    if( isdot & LCD_HASH){
        tData |= 0x80;
    }
    SPI_OneClock();
    SPI_Transmit(tData);
    LCD_Load();

}
void LCD_Transmit(char cData)
{
    char tData = 0;
    if (cData < sizeof(CHARTABLE)/sizeof(CHARTABLE[0]))
    {
        tData = pgm_read_byte(&CHARTABLE[(int)cData]);
    }
    
    SPI_SetData(0x00);
    SPI_OneClock();
    SPI_Transmit(tData);
    LCD_Load();
}

void LCD_Clear()
{
    char i;
    for(i=0; i<8; i++)
    {
        LCD_Transmit(255);
    }
}
