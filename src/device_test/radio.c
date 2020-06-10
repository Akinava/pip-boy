#include "radio.h"

uint8_t radio_test(void){
  // Set MOSI, SCK, CS as Output
  SPI_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  RADIO_DDR |= _BV(RADIO_CSN);
  RADIO_DDR |= _BV(RADIO_CE);
  // set radio csn off
  SPI_UNSET(RADIO_PORT, RADIO_CSN);
  SET_LOW(RADIO_PORT, RADIO_CE);
  // set sd cs off
  SPI_UNSET(SD_PORT, SD_CS);
  // Enable SPI, Set as Master
  //Prescaler: Fosc/16, Enable Interrupts
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0); // | (1 << SPR1)
  
  //use max SPI frequency
  //SPCR &= ~((1 << SPR1) | (1 << SPR0)); // f_OSC/4
  //SPSR |= (1 << SPI2X); // Doubled Clock Frequency: f_OSC/2
  //SD_UNSET(SD_PORT, SD_CS);
  
  spi_send(RADIO_NOOP);
  SPI_SET(RADIO_PORT, RADIO_CSN);
  _delay_ms(120);

  // send noop
  spi_send(RADIO_NOOP);
  uint8_t res = SPDR;
  SPI_UNSET(RADIO_PORT, RADIO_CSN);

  if (res == RADIO_DEFAULT_STATE){
    return 1;
  }
  return 0;
}

void spi_send(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}
