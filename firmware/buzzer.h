#ifndef BUZZER_H
#define BUZZER_H

/**
 * Buzzer driver header file
 */

#define BUZZER_DDR DDRD
#define BUZZER_PORT PORTD
#define BUZZER_BIT PORTD3

void inline Buzzer_Init();
void inline Buzzer_On();
void inline Buzzer_Off();
void inline Buzzer_Disable();

#endif /*BUZZER_H*/