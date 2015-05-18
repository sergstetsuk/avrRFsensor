#ifndef SPI_H
#define SPI_H

#define USICR_DEF (0<<USISIE)|(0<<USIOIE)|(0<<USIWM1)|(1<<USIWM0) \
            |(0<<USICS1)|(0<<USICS0)|(0<<USICLK)|(1<<USITC) //Software strobe

void inline SPI_Init();
void inline SPI_Disable();
void inline SPI_SetData(char cData);
void inline SPI_OneClock();
char SPI_Transmit (char cData);

#endif /*SPI_H*/