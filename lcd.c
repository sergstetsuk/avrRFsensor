#include <avr/io.h>

#include "lcd.h"

const unsigned char CHARTABLE[] = {
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

void LCD_Transmit(char cData)
{
    int tData = 0;
    if (cData < sizeof(CHARTABLE)/sizeof(CHARTABLE[0]))
    {
	tData = CHARTABLE[cData];
    }
    
    USIDR = 0;
    USICR = (0<<USISIE)|(0<<USIOIE)|(0<<USIWM1)|(1<<USIWM0)
            |(0<<USICS1)|(0<<USICS0)|(0<<USICLK)|(1<<USITC);
    USICR = (0<<USISIE)|(0<<USIOIE)|(0<<USIWM1)|(1<<USIWM0)
            |(0<<USICS1)|(0<<USICS0)|(1<<USICLK)|(1<<USITC);
/*  USIDR = tData;
    for(i=0; i<8; i++)
    {
        //~ if(tData & 0x01) PORTB |= LCD_DATA;
        //~ else PORTB &= ~LCD_DATA;
        //~ PORTB ^= LCD_CLCK; //Clock
        //~ PORTB ^= LCD_CLCK; //Clock
        //~ tData >>= 1;
    USICR = (0<<USISIE)|(0<<USIOIE)|(0<<USIWM1)|(1<<USIWM0)
            |(0<<USICS1)|(0<<USICS0)|(0<<USICLK)|(1<<USITC);
    USICR = (0<<USISIE)|(0<<USIOIE)|(0<<USIWM1)|(1<<USIWM0)
            |(0<<USICS1)|(0<<USICS0)|(1<<USICLK)|(1<<USITC);
    }
*/
    SPI_Transmit(tData);
    PORTB ^= LCD_LOAD;  //Load data
    PORTB ^= LCD_LOAD;
}

void LCD_Clear()
{
    char i;
    for(i=0; i<8; i++)
    {
        LCD_Transmit(255);
    }
}
