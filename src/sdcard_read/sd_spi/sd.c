#include "sd.h"

uint8_t sd_begin(void){
  if (!card_init()){return 0;}
  if (!vol_init()){return 0;}
  return 1;
}

uint8_t vol_init(void){
  uint32_t start_block = 0;
  uint8_t buf[VOL_ADDRESS_COUNT];
  if (!cd_raw_read(start_block, VOL_ADDRESS_OFFSET, buf, VOL_ADDRESS_COUNT)){return 0;}

  uint32_t vol_address = *((uint32_t*)buf);

  for (uint8_t i=0; i<VOL_ADDRESS_COUNT; i++){
    uint8_t t = (vol_address>>(8*i))&0xff;
    displayPrintHex((uint8_t)t, i%16, i/16);
  }
  displayUpdate();


  return 1;
}

uint8_t card_init(void){
  uint8_t ocr[4];
  uint8_t slow = 0;
  // Set MOSI, SCK, CS as Output
  SD_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // set sd cs off
  SET_HIGH(SD_PORT, SD_CS);
  // Enable SPI, Set as Master
  //Prescaler: Fosc/16, Enable Interrupts
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0); // | (1 << SPR1)
  uint8_t r = card_command(CMD0, 0, 0X95);  // 0x3f

  for (uint16_t retry = 0; r != R1_IDLE_STATE; retry++){
    if (retry == 0XFFFF) {
      return 0;
    }
    r = spi_rec();
  }
  r = card_command(CMD8, 0x1AA, 0X87);
  if (r != 1){
    return 0;
  }
  type_ = SD_CARD_TYPE_SD2; 


  for (uint16_t retry = 0; ; retry++) {
    card_command(CMD55, 0, 0XFF);
    r = card_command(ACMD41, type_ == SD_CARD_TYPE_SD2 ? 0X40000000 : 0, 0XFF);
    if (r == R1_READY_STATE)break;
    if (retry == 1000) {
      return 0;
    }
  }
  if(card_command(CMD58, 0, 0XFF)) {
      return 0;
  }
  for (uint8_t i = 0; i < 4; i++) ocr[i] = spi_rec();
  if (type_ == SD_CARD_TYPE_SD2 && (ocr[0] & 0XC0) == 0xC0) type_ = SD_CARD_TYPE_SDHC;
  //use max SPI frequency
  SPCR &= ~((1 << SPR1) | (1 << SPR0)); // f_OSC/4
  if (!slow) SPSR |= (1 << SPI2X); // Doubled Clock Frequency: f_OSC/2
  SET_HIGH(SD_PORT, SD_CS);
  
  return 1;
}

void spi_send(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_rec(void){
  SPDR = 0xFF;
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}


uint8_t card_command(uint8_t cmd, uint32_t arg, uint8_t crc){
  uint8_t r1;
  // end read if in partialBlockRead mode
  read_end();
  //select card
  SET_LOW(SD_PORT, SD_CS);
  // some cards need extra clocks to go to ready state
  spi_rec();
  // send command
  spi_send(cmd | 0x40);
  //send argument
  for (int8_t s = 24; s >= 0; s -= 8) spi_send(arg >> s);
  //send CRC
  spi_send(crc);
  //wait for not busy
  for (uint8_t retry = 0; (r1 = spi_rec()) == 0xFF && retry != 0XFF; retry++);
  return r1;
}

void read_end(void){
  if (in_block_) {
    // skip data and crc
    SPDR = 0XFF;
    while (offset_++ < 513) {
      while(!(SPSR & (1 << SPIF)));
      SPDR = 0XFF;
    }
    while(!(SPSR & (1 << SPIF)));//wait for last crc byte
    SET_HIGH(SD_PORT, SD_CS);
    in_block_ = 0;
  }
}

uint8_t sd_wait_start_block(void){
  uint8_t r;
  uint16_t retry;
  //wait for start of data
  for (retry = 0; ((r = spi_rec()) == 0XFF) && retry != 10000; retry++);
  if (r == DATA_START_BLOCK) return 1;
  return 0;
}

uint8_t cd_raw_read(uint32_t block, uint16_t offset, uint8_t *dst, uint16_t count)
{
  if (count == 0) return 1;
  if ((count + offset) > 512) {
    return 0;
  }
  if (!in_block_ || block != block_ || offset < offset_) {
    block_ = block;
    //use address if not SDHC card
    if (type_ != SD_CARD_TYPE_SDHC) block <<= 9;
    if (card_command(CMD17, block, 0XFF)) {
      return 0;
    }
    if (!sd_wait_start_block()) return 0;
    offset_ = 0;
    in_block_ = 1;
  }
  //start first spi transfer
  SPDR = 0XFF;
  //skip data before offset
  for (;offset_ < offset; offset_++) {
    while(!(SPSR & (1 << SPIF)));
    SPDR = 0XFF;
  }
  //transfer data
  uint16_t n = count - 1;
  for (uint16_t i = 0; i < n; i++) {
    while(!(SPSR & (1 << SPIF)));
    dst[i] = SPDR;
    SPDR = 0XFF;
  }
  while(!(SPSR & (1 << SPIF)));// wait for last byte
  dst[n] = SPDR;
  offset_ += count;
  if (!partial_block_read_ || offset_ >= 512) read_end();
  return 1;
}
