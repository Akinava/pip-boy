#include "sd.h"

uint8_t sd_find_obj(uint32_t start_sector, const char* obj_name, uint32_t* obj_sector){
  displayBegin();
  displayClean();
  show_u32(root_sector_, 0, 0);
  displayUpdate();
  return 1; 


  /*
  uint8_t obj_info_buf[OBJECT_RECORD_SIZE];
  uint16_t claster_flag = 0; 
  uint16_t max_dir_entry_count;

  if (start_sector == 0){
    start_sector = root_sector_;
    max_dir_entry_count = root_entry_count_;
  }else{
    max_dir_entry_count = bytes_per_sector_ * sectors_per_claster_ / OBJECT_RECORD_SIZE;
  }

  displayBegin();
  displayClean();
  show_u8(0, 0, 8);
  displayUpdate();
 

  while(claster_flag < LAST_CLASTER_FLAG){
    for (uint16_t entry_index=0; entry_index<max_dir_entry_count; entry_index++){
      if (!cd_raw_read(start_sector, entry_index*OBJECT_RECORD_SIZE, obj_info_buf, OBJECT_RECORD_SIZE)){return 0;}

      displayBegin();
      displayClean();
      for(uint8_t i=1; i<OBJECT_RECORD_SIZE; i++){
        show_u8(obj_info_buf[i], i%16, i/16);
      }
      displayUpdate();
      return 0;
 
    } 
  }
  return 1;
  */
}

uint8_t sd_init(void){
  if (!card_init()){return 0;}
  if (!vol_init()){return 0;}
  return 1;
}

uint8_t vol_init(void){
  uint8_t vol_address_buf[SECTOR_LENGTH];
  if (!cd_raw_read(0, VOL_ADDRESS_OFFSET, vol_address_buf, SECTOR_LENGTH)){return 0;}
  memcpy(&volume_sector_, vol_address_buf, sizeof(volume_sector_));

  uint8_t vol_info_buf[VOL_INFO_SIZE];
  if (!cd_raw_read(volume_sector_, VOL_INFO_OFFSET, vol_info_buf, VOL_INFO_SIZE)){return 0;}

  // MIGHT BE OPTIMIZED
  memcpy(&bytes_per_sector_, vol_info_buf, sizeof(bytes_per_sector_));
  memcpy(&sectors_per_claster_, &vol_info_buf[SECTORS_PER_CLASTER_OFFSET], sizeof(sectors_per_claster_));
  memcpy(&reserved_sectors_, &vol_info_buf[RESERVED_SECTORS_OFFSET], sizeof(reserved_sectors_));
  memcpy(&fat_count_, &vol_info_buf[FAT_COUNT_OFFSET], sizeof(fat_count_));
  memcpy(&root_entry_count_, &vol_info_buf[ROOT_ENTRY_COUNT_OFFSET], sizeof(root_entry_count_));
  memcpy(&sectors_per_fat_, &vol_info_buf[SECTORS_PER_FAT_OFFSET], sizeof(sectors_per_fat_));

  fat_sector_ = volume_sector_ + reserved_sectors_; 
  root_sector_ = fat_sector_ + sectors_per_fat_ * fat_count_;
  data_sector_ = root_sector_ + root_entry_count_ * OBJECT_RECORD_SIZE / bytes_per_sector_;

  show_u32(volume_sector_, 0, 0);
  show_u16(bytes_per_sector_, 0, 1);
  show_u8(sectors_per_claster_, 0, 2);

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
