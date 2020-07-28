#include "spi.h"

void spi_init(void){
  SPI_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  SPI_UNSET(SPI_PORT, CS);
  // SPI enable
  SPCR |= _BV(SPE);
  // set device as master
  SPCR |= _BV(MSTR);

}

void spi_set_speed(uint8_t clock){
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (clock & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((clock >> 2) & SPI_2XCLOCK_MASK);

}

void spi_send(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}
  
uint8_t spi_receive(void){
  spi_send(0xff);
  return SPDR;
}
