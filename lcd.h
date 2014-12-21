#ifndef LCD_H
#define LCD_H

/**
 * LCD driver header file for TIC8148 LCD
 */

#define LCD_DATA (1<<PORTB0)
#define LCD_CLCK (1<<PORTB2)
#define LCD_LOAD (1<<PORTB3)

void LCD_Transmit (char);

#endif /*LCD_H*/