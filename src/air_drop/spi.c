#include "spi.h"

void spi_init(void){
  SPI_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  SPI_UNSET(SPI_PORT, CS);
}

void spi_send(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}
