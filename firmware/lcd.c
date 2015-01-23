#include <avr/io.h>
#include <avr/pgmspace.h>

#include "spi.h"
#include "lcd.h"

const unsigned char CHARTABLE[] PROGMEM = {
      0b01111011,//0b11011110, //0
      0b01100000,//0b00000110, //1
      0b00110111,//0b11101100, //2
      0b01110101,//0b10101110, //3
      0b01101100,//0b00110110, //4
      0b01011101,//0b10111010, //5
      0b01011111,//0b11111010, //6
      0b01110000,//0b00001110, //7
      0b01111111,//0b11111110, //8
      0b01111101,//0b10111110, //9
      0b01111110,//0b01111110, //A
      0b01001111,//0b11110010, //B
      0b00011011,//0b11011000, //C
      0b01100111,//0b11100110, //D
      0b00011111,//0b11111000, //E
      0b00011110,//0b01111000, //F
                      };

void inline LCD_Init()
{
    DDRB |= LCD_LOAD;   //Configure LOAD pin as output
    //PORTB &= ~LCD_LOAD;
}

void inline LCD_Load()
{
    PORTB ^= LCD_LOAD;  //Load data strobe
    PORTB ^= LCD_LOAD;
}
void LCD_TransmitDot(char cData)
{
    char tData = 0;
    if (cData < sizeof(CHARTABLE)/sizeof(CHARTABLE[0]))
    {
        tData = CHARTABLE[cData];
    }
    
    SPI_SetData(0x80);
    SPI_OneClock();
    SPI_Transmit(tData);
    LCD_Load();

}
void LCD_Transmit(char cData)
{
    char tData = 0;
    if (cData < sizeof(CHARTABLE)/sizeof(CHARTABLE[0]))
    {
        tData = pgm_read_byte(&CHARTABLE[cData]);
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
