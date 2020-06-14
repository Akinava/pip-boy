#include "spi_writer.h"

int main(void){
  display_begin();
  display_clean();
  print("Hello SPI", 0, 0);
  _delay_ms(100);
  // Set MOSI, SCK, SD_CS as Output B
  SPI_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // set MISO as Input
  SET_DDR_IN(SPI_DDR, MISO);
  // Set RADIO_CS, SPI_MASTER as Output D
  RADIO_DDR |= _BV(RADIO_CSN)|_BV(SPI_MASTER_PIN);


  // Enable SPI, Set as Master
  SPCR = _BV(SPE)|_BV(MSTR);
  //Prescaler: Fosc/4, by default
  //SET_LOW(SPI_MASTER_PORT, SPI_MASTER_PIN);
  //Prescaler: Fosc/16
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0);

  // off other spi hw
  SPI_UNSET(SD_PORT, SD_CS);
  SPI_UNSET(RADIO_PORT, RADIO_CSN);

  // PULSE
  SET_LOW(SPI_PORT, SCK);
  SET_HIGH(SPI_MASTER_PORT, SPI_MASTER_PIN);
  _delay_ms(10);
  SET_LOW(SPI_MASTER_PORT, SPI_MASTER_PIN);
  _delay_ms(25);

  program_enable();
  read_signature();
  erise_chip();
  load_program(); 
  //write_fuse(WRITE_HIGH_FUSE, 0xda); // def 0xda / 0xdf
  //write_fuse(WRITE_EXT_FUSE, 0xfd);  // def 0xfd / 0xff
  //write_fuse(WRITE_LOW_FUSE, 0xff);  // def 0xff / 0xfe

  _delay_ms(20);
  print("DONE", 0, 7);
  // off spi
  //SET_HIGH(SPI_MASTER_PORT, SPI_MASTER_PIN); // TODO try whithout it
  //_delay_ms(20);                             // TODO 
  SET_LOW(SPI_MASTER_PORT, SPI_MASTER_PIN);
  SET_LOW(SPI_PORT, SCK);
  SET_LOW(SPI_PORT, MOSI);

  while(1);
}

void spi_send(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

void program_enable(void){
  spi_send(PROGRAM_ENABLE);
  spi_send(PROGRAM_ACKNOWLEDGE);
  spi_send(0x00);
  if (SPDR != PROGRAM_ACKNOWLEDGE){
    print("Program mode fail", 0, 1);
    while(1);
  }
  spi_send(0x00);
}

void read_signature(void){
  clean_buf();
  for (uint8_t i=0; i<3; i++){
    isp_command(READ_SIGNATURE, 0, i, 0);
    print8(SPDR, i*20, 1); // 0x1e, 0x95, 0x0f
  }
}

void erise_chip(void){
  isp_command(PROGRAM_ENABLE, CHIP_ERASE, 0, 0);
}

void write_fuse(uint8_t fuse, uint8_t value){
  isp_command(PROGRAM_ENABLE, fuse, 0, value);
}

void load_program(void){
  // SPM_PAGESIZE 128
  //uint32_t address = 0;
  for (uint8_t i=0; i<128; i++){
    isp_command(LOAD_PROGRAM_LOW_BYTE, 0, i, i); 
  }
  isp_command(WRITE_PAGE, 0, 0, 0); 
}

void isp_command(uint8_t cmd0, uint8_t cmd1, uint8_t cmd2, uint8_t cmd3){
  spi_send(cmd0);
  spi_send(cmd1);
  spi_send(cmd2);
  spi_send(cmd3);
}
