#include "sd.h"

const uint8_t cluster_bytes_rule[] PROGMEM = {0x1a, 0x1b, 0x14, 0x15};
const uint8_t size_bytes_rule[] PROGMEM = {0x1c, 0x1d, 0x1e, 0x1f};

uint8_t file_read(file_t* file, uint8_t* buf, uint16_t size){
  uint32_t sector = file->sector + file->cursor / vol_info_.bytes_per_sector;
  uint16_t offset = file->cursor % vol_info_.bytes_per_sector;
  if (!cd_raw_read_(sector, offset, buf, size)){return 0;}
  file->cursor += size;
  return 1;
}

uint8_t file_open(const char* file_path, file_t* file){
  // parameters:
  // file_path - in unix view exp: '/BIN/APP.BIN'
  // file      - file sector, file size
  //
  // if file exist save in file file sector and size; return 1
  // else retrn 0
  uint8_t c;
  uint8_t i = 0;
  uint8_t obj_i = 0;
  file->sector = root_sector_;
  file->cursor = 0;

  do{
    c = pgm_read_byte_near(file_path + i);
    if (c==CHAR_SLASH || !c){
      if (i){
      if (!find_obj_by_name(obj_name_, file)){return 0;}
      }
      obj_i = 0;
      memset_(obj_name_, CHAR_SPACE, OBJECT_NAME_SIZE);
    }else{
      if (c == CHAR_DOT){
        obj_i = 8;
      }else{
        obj_name_[obj_i] = c;
        obj_i++;
      }
    }
    i++;
  }while(c);
  return 1;
}

uint8_t sd_init(void){
  if (!card_init_()){return 0;}
  if (!vol_init_()){return 0;}
  return 1;
}

uint8_t find_obj_by_name(uint8_t* obj_name, file_t* file){
  // parameters:
  // obj_name - object name and ext in fat16 format cahr[8+3], exp: "APP     BIN"
  // file     -  file sector and file size
  uint16_t directory_entries = vol_info_.root_directory_entries;
  if (file->sector != root_sector_){
     directory_entries = vol_info_.sectors_per_claster * vol_info_.bytes_per_sector / OBJECT_RECORD_SIZE; 
  }

  uint8_t obj_buf[OBJECT_RECORD_SIZE];
  uint8_t records_per_sector = vol_info_.bytes_per_sector / OBJECT_RECORD_SIZE;

  do{
    for (uint16_t i=0; i<directory_entries; i++){
      uint32_t rec_sector = file->sector + i / records_per_sector;
      uint16_t rec_offset = i % records_per_sector * OBJECT_RECORD_SIZE;

      if (!cd_raw_read_(rec_sector, rec_offset, obj_buf, OBJECT_RECORD_SIZE)){return 0;}

      if (cmp_(obj_buf, obj_name)){
        file_info_parce_(file, obj_buf);
        return 1;
      }
    }
  }while(next_claster_(file));
  return 0;
}

uint8_t next_claster_(file_t* file){
  if (file->sector == root_sector_){return 0;}
  uint8_t* cluster_buf = ((uint8_t*)&file->cluster);
  uint16_t fat_cluster_size = sizeof(file->cluster);
  uint16_t fat_cluster_place = file->cluster*fat_cluster_size;

  if (!cd_raw_read_(fat_sector_, fat_cluster_place, cluster_buf, fat_cluster_size)){return 0;}
  if (file->sector >= END_OF_CLASTERCHAIN){return 0;}

  get_sector_by_cluster_(file);
  return 1;
}


void file_info_parce_(file_t* file, uint8_t* file_info){
  file->cluster = warp_bytes_(file_info, cluster_bytes_rule);
  file->size = warp_bytes_(file_info, size_bytes_rule);
  get_sector_by_cluster_(file);
}

void get_sector_by_cluster_(file_t* file){
  file->sector = data_sector_ + ((file->cluster-2) * vol_info_.sectors_per_claster);
}

uint32_t warp_bytes_(uint8_t* file_info, const uint8_t* rule){
  uint32_t res = 0;
  for (uint8_t i=0; i<4; i++){
    uint8_t r = pgm_read_byte_near(rule + i);
    res |= file_info[r] << (8*i);
  }
  return res;
}

void memset_(uint8_t* s1, uint8_t c, uint8_t size){
  for (uint8_t i=0; i<size; i++){
    s1[i] = c;
  }
}

uint8_t cmp_(uint8_t* s1, uint8_t* s2){
  for (uint8_t i=0; i< OBJECT_NAME_SIZE; i++){
    if (s1[i] != s2[i]){return 0;}
  }
  return 1;
}

uint8_t vol_init_(void){
  uint8_t* vol_address_buf = ((uint8_t*)&volume_sector_);
  if (!cd_raw_read_(0, VOL_ADDRESS_OFFSET, vol_address_buf, SECTOR_LENGTH)){return 0;}

  uint8_t* vol_info_buf = ((uint8_t*)&vol_info_);
  if (!cd_raw_read_(volume_sector_, VOL_INFO_OFFSET, vol_info_buf, sizeof(vol_info_))){return 0;}
  fat_sector_ = volume_sector_ + vol_info_.reserved_sectors; 
  root_sector_ = fat_sector_ + vol_info_.sectors_per_FAT * vol_info_.number_of_FATs;
  data_sector_ = root_sector_ + vol_info_.root_directory_entries * OBJECT_RECORD_SIZE / vol_info_.bytes_per_sector;
  return 1;
}

uint8_t card_init_(void){
  uint8_t ocr[4];
  uint8_t slow = 0;
  // Set MOSI, SCK, CS as Output
  SD_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // set sd cs off
  SET_HIGH(SD_PORT, SD_CS);
  // Enable SPI, Set as Master
  //Prescaler: Fosc/16, Enable Interrupts
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0); // | (1 << SPR1)
  uint8_t r = card_command_(CMD0, 0, 0X95);  // 0x3f

  for (uint16_t retry = 0; r != R1_IDLE_STATE; retry++){
    if (retry == 0XFFFF) {
      return 0;
    }
    r = spi_rec_();
  }
  r = card_command_(CMD8, 0x1AA, 0X87);
  if (r != 1){
    return 0;
  }
  type_ = SD_CARD_TYPE_SD2; 


  for (uint16_t retry = 0; ; retry++) {
    card_command_(CMD55, 0, 0XFF);
    r = card_command_(ACMD41, 0X40000000, 0XFF);
    if (r == R1_READY_STATE)break;
    if (retry == 1000) {
      return 0;
    }
  }
  if (card_command_(CMD58, 0, 0XFF)) {
      return 0;
  }
  for (uint8_t i = 0; i < 4; i++) ocr[i] = spi_rec_();
  type_ = SD_CARD_TYPE_SDHC;
  //use max SPI frequency
  SPCR &= ~((1 << SPR1) | (1 << SPR0)); // f_OSC/4
  if (!slow) SPSR |= (1 << SPI2X); // Doubled Clock Frequency: f_OSC/2
  SET_HIGH(SD_PORT, SD_CS);
  
  return 1;
}

void spi_send_(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_rec_(void){
  SPDR = 0xFF;
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}


uint8_t card_command_(uint8_t cmd, uint32_t arg, uint8_t crc){
  uint8_t r1;
  // end read if in partialBlockRead mode
  read_end_();
  //select card
  SET_LOW(SD_PORT, SD_CS);
  // some cards need extra clocks to go to ready state
  spi_rec_();
  // send command
  spi_send_(cmd | 0x40);
  //send argument
  for (int8_t s = 24; s >= 0; s -= 8) spi_send_(arg >> s);
  //send CRC
  spi_send_(crc);
  //wait for not busy
  for (uint8_t retry = 0; (r1 = spi_rec_()) == 0xFF && retry != 0XFF; retry++);
  return r1;
}

void read_end_(void){
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

uint8_t sd_wait_start_block_(void){
  uint8_t r;
  uint16_t retry;
  //wait for start of data
  for (retry = 0; ((r = spi_rec_()) == 0XFF) && retry != 10000; retry++);
  if (r == DATA_START_BLOCK) return 1;
  return 0;
}

uint8_t cd_raw_read_(uint32_t block, uint16_t offset, uint8_t *dst, uint16_t count)
{
  if (count == 0) return 1;
  if ((count + offset) > 512) {
    return 0;
  }
  if (!in_block_ || block != block_ || offset < offset_) {
    block_ = block;
    //use address if not SDHC card
    if (type_ != SD_CARD_TYPE_SDHC) block <<= 9;
    if (card_command_(CMD17, block, 0XFF)) {
      return 0;
    }
    if (!sd_wait_start_block_()) return 0;
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
  if (!partial_block_read_ || offset_ >= 512) read_end_();
  return 1;
}
