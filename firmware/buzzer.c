#include <avr/io.h>
#include "buzzer.h"

void inline Buzzer_Init() {
    Buzzer_On();
    BUZZER_DDR |= (1<<BUZZER_BIT);
}

void inline Buzzer_On() {
    BUZZER_PORT &= ~(1<<BUZZER_BIT);
}

void inline Buzzer_Off() {
    BUZZER_PORT |= (1<<BUZZER_BIT);
}

void inline Buzzer_Disable() {
    Buzzer_Off();
    BUZZER_DDR &= ~(1<<BUZZER_BIT);
}
