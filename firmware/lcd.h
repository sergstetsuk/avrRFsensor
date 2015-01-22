#ifndef LCD_H
#define LCD_H

/**
 * LCD driver header file for TIC8148 LCD
 */

#define LCD_DATA (1<<PORTB1)
#define LCD_CLCK (1<<PORTB2)
#define LCD_LOAD (1<<PORTB3)

void inline LCD_Init();
void inline LCD_Load();
void LCD_Transmit(char);
void LCD_TransmitDot(char);
void LCD_Clear();

#endif /*LCD_H*/