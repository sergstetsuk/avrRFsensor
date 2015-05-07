#ifndef LCD_H
#define LCD_H

/**
 * LCD driver header file for TIC8148 LCD
 */

#define LCD_DOT 0x01
#define LCD_HASH 0x02
#define LCD_RAW 0x04

void inline LCD_Init();
void inline LCD_Load();
void LCD_Transmit(char);
void LCD_TransmitDot(char,char);
void LCD_Clear();

#endif /*LCD_H*/