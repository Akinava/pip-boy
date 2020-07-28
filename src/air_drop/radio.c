#include "radio.h"

uint8_t radio_init(void){
  // set radio csn off
  // set sd cs off
  SET_DDR_OUT(RADIO_DDR, RADIO_CSN);
  SET_DDR_OUT(RADIO_DDR, RADIO_CE);
  SPI_UNSET(RADIO_PORT, RADIO_CSN);
  SET_LOW(RADIO_PORT, RADIO_CE);
 
  // Enable SPI, Set as Master
  //Prescaler: Fosc/16, Enable Interrupts
  // Set MOSI, SCK, CS as Output
  spi_init();
  spi_set_speed(SPI_CLOCK_DIV16);

  //spi_send(RADIO_NOOP);
  SPI_SET(RADIO_PORT, RADIO_CSN);
  _delay_ms(120);

  // send noop
  uint8_t res = spi_receive();
  SPI_UNSET(RADIO_PORT, RADIO_CSN);

  if (res == RADIO_DEFAULT_STATE){
    return 1;
  }
  return 0;
}
