#include <avr/io.h>

#include "lcd.h"

const int CHARTABLE[] = {
      0b110111100, //0
      0b000001100, //1
      0b111011000, //2
      0b101011100, //3
      0b001101100, //4
      0b101110100, //5
      0b111110100, //6
      0b000011100, //7
      0b111111100, //8
      0b101111100, //9
      0b011111100, //A
      0b111100100, //B
      0b110110000, //C
      0b111001100, //D
      0b111110000, //E
      0b011110000, //F
                      };

void LCD_Transmit (char cData)
{
    int tData = CHARTABLE[cData];
    int i;
    for(i=0; i<9; i++)
    {
        if(tData & 0x01) PORTB |= LCD_DATA;
        else PORTB &= ~LCD_DATA;
        PORTB ^= LCD_CLCK; //Clock
        PORTB ^= LCD_CLCK; //Clock
        tData >>= 1;
    }
    PORTB ^= LCD_LOAD;  //Load data
    PORTB ^= LCD_LOAD;
}
