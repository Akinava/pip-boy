#include "spi_writer.h"

int main(void){
  display_begin();
  display_clean();
  print("Hello SPI", 0, 0);

  // Set MOSI, SCK, SD_CS as Output B
  SPI_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // Set RADIO_CS, SPI_MASTER as Output D
  RADIO_DDR |= _BV(RADIO_CSN)|_BV(SPI_MASTER_PIN);

  SPI_UNSET(SD_PORT, SD_CS);
  SPI_UNSET(RADIO_PORT, RADIO_CSN);
  SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);

  // Enable SPI, Set as Master
  SPCR = _BV(SPE)|_BV(MSTR);
  //Prescaler: Fosc/4, by default
  //SPI_SET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  //Prescaler: Fosc/16
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0);

  print("Write SPI", 0, 1);

  // prog enable
  spi_send(0xac);
  spi_send(0x53);
  spi_send(0x00);
  spi_send(0x00);

  // chip erase
  spi_send(0xca);
  spi_send(0x80);
  spi_send(0x00);
  spi_send(0x00);

  SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);

  print("DONE", 0, 2);
  
  while(1);
}

void spi_send(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}
