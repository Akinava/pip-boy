#include "boot.h"

int main(void){

  MCUSR = ~(_BV(WDRF));
  watchdog_config_(WATCHDOG_RESET);

  // MAIN LOGIC
  setup_button_();
  
  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    setup_led_();
    SET_HIGH(LED_PORT, LED_PIN);

    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){};
    SET_LOW(LED_PORT, LED_PIN);

    load_default_app();
  }
  app_start();
}

void load_default_app(void){
  load_app_by_name(BOOT_APP);
}

void load_app_by_name(const char* file_path){
  setup_led_();

  if (!sd_init()){
    error_light_();
  }
 
  if (!find_file_by_path(file_path)){
    error_blink_();
  }
  load_app_by_cluster(file.cluster, file.size);
}

void load_app_by_cluster(uint16_t cluster, uint32_t size){
  setup_led_();
  if (!sd_init()){error_light_();}

  uint32_t sector = get_sector_by_cluster_(cluster);
  uint32_t address = 0;
  uint8_t page_cursor = SPM_PAGESIZE;

  while(1){
    uint16_t sector_offset = address % SECTOR_BUFFER_SIZE;
    // read next sector
    if (sector_offset == 0){
      if (!read_sector_(sector)){error_blink_();}
      sector++;
    }
    // read word
    uint16_t temp_word = *((uint16_t*)(sector_buffer + sector_offset));
    boot_page_fill(address, temp_word);

    address += 2;
    page_cursor -= 2;

    // write page
    if (page_cursor == 0){
      show_status(temp_word);

  	  // Perform page erase
      boot_page_erase(address-SPM_PAGESIZE);
	    // Wait until the memory is erased
      boot_spm_busy_wait();		
      boot_page_write(address-SPM_PAGESIZE);
	    // wait until finished writing
      boot_spm_busy_wait();

      // Re-enable the RWW section 
      //boot_rww_enable();
      page_cursor = SPM_PAGESIZE;
      if (address >= size){break;}
    }
  }

  SET_HIGH(LED_PORT, LED_PIN);
  _delay_ms(100);
  SET_LOW(LED_PORT, LED_PIN);

  // reset
  watchdog_config_(WATCHDOG_125MS);
	while(1);
 }

static inline void setup_button_(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

static inline void show_status(uint16_t status){
  if(status&1){
    SET_HIGH(LED_PORT, LED_PIN);
  }else{
    SET_LOW(LED_PORT, LED_PIN);
  }
}

static inline void setup_led_(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}

static inline void error_light_(void){
  SET_HIGH(LED_PORT, LED_PIN);
  while(1);
}

void error_blink_(void){
  while(1){
    TOGGLE(LED_PORT, LED_PIN);
    _delay_ms(500);
  }
}

static inline void watchdog_config_(uint8_t x){
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = x;
}

// ===== SD ===== //

static inline uint8_t find_file_by_path(const char* file_path){
  // parameters:
  // file_path - in unix view exp: '/BIN/APP.BIN'
  // file      - file sector, file size
  //
  // if file exist save in file file sector and size; return 1
  // else retrn 0

  file.sector = root_sector_;
  file.cluster = ROOT_CLUSTER;
  uint8_t file_path_cursor = 0; // zero sybmol is a slash '/'

  while(copy_file_name(&file_path_cursor, file_path)){
    if (!find_obj_by_name()){
      return 0;
    }
    if (!pgm_read_byte(file_path + file_path_cursor)){return 1;}
  }
  return 0;
}

static inline uint8_t copy_file_name(uint8_t* file_path_cursor, const char* file_path){
  erase_obj_name_();
  uint8_t name_index = 0;
  uint8_t c;

  do{
    (*file_path_cursor)++;
    c = pgm_read_byte(file_path + *file_path_cursor);
    if (!c || c == CHAR_SLASH){break;}
    if (c == CHAR_DOT){
      name_index = OBJECT_NAME_SIZE;
    }else{
      obj_name_[name_index] = c;
      name_index++;
    }
  }while(1);
  return 1;
}

static inline uint8_t find_obj_by_name(void){
  uint16_t sector_offset;

  do{
    if(!read_sector_(file.sector)){return 0;}
    // go through sector
    for (sector_offset=0; sector_offset<vol_info.bytes_per_sector; sector_offset+=OBJECT_RECORD_SIZE){
      // if record_is_empty
      if (!*(sector_buffer+sector_offset)){return 0;}
      if(compare_name(sector_buffer+sector_offset)){
        save_obj_to_file(sector_buffer+sector_offset);
        return 1;
      }
    }
  }while(next_sector());
  return 0;
}

static inline void save_obj_to_file(uint8_t* buf){
  file.cluster = *((uint16_t*)(buf+FILE_CLUSTER_OFFSET));
  file.size = *((uint32_t*)(buf+FILE_SIZE_OFFSET));
  file.sector = get_sector_by_cluster_(file.cluster);
}

static inline void erase_obj_name_(void){
  for (uint8_t i=0; i<OBJECT_NAME_SIZE+OBJECT_EXT_SIZE; i++){
    obj_name_[i] = CHAR_SPACE;
  }
}

uint8_t compare_name(uint8_t* buf){
  for (uint8_t i=0; i<OBJECT_NAME_SIZE+OBJECT_EXT_SIZE; i++){
    if (*(buf+i) != *(obj_name_+i)){return 0;}
  }
  return 1;
}

static inline uint8_t next_sector(void){
  file.sector++;

  if (file.cluster == ROOT_CLUSTER){
    //                 /                                     root sectors                                            /
    if (file.sector <= root_sector_ + vol_info.root_directory_entries * OBJECT_RECORD_SIZE / vol_info.bytes_per_sector){
      return 1;
    }
  }else{
    //                 last sector in cluster
    if (file.sector <= get_sector_by_cluster_(file.cluster) + vol_info.sectors_per_claster){
      return 1;
    }
    if(next_cluster_()){return 1;}
  }
  return 0;
}

static inline uint8_t next_cluster_(void){
  uint16_t fat_cluster_place = file.cluster * sizeof(file.cluster);
  if (!read_sector_(fat_sector_)){return 0;}
  file.cluster = *((uint32_t*)(sector_buffer + fat_cluster_place));
  if (file.sector >= END_OF_CLASTERCHAIN){return 0;}
  file.sector = get_sector_by_cluster_(file.cluster);
  return 1;
}

uint32_t get_sector_by_cluster_(uint16_t cluster){
  return data_sector_ + ((cluster-2) * vol_info.sectors_per_claster);
}

//============================== sd func ====================================//

static inline uint8_t sd_init(void){
  if (!card_init_()){return 0;}
  if (!vol_init_()){return 0;}
  return 1;
}

static inline uint8_t vol_init_(void){
  // volume address
  if(!read_sector_(0)){return 0;}
  volume_sector_ = *((uint32_t*)(sector_buffer + VOL_ADDRESS_OFFSET));
  // volume info
  if(!read_sector_(volume_sector_)){return 0;}
  vol_info = *((vol_info_t*)(sector_buffer + VOL_INFO_OFFSET));

  fat_sector_ = volume_sector_ + vol_info.reserved_sectors; 
  root_sector_ = fat_sector_ + vol_info.sectors_per_FAT * vol_info.number_of_FATs;
  data_sector_ = root_sector_ + vol_info.root_directory_entries * OBJECT_RECORD_SIZE / vol_info.bytes_per_sector;
  return 1;
}

static inline uint8_t card_init_(void){
  // Set MOSI, SCK, CS as Output
  SD_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // set sd cs off
  SD_UNSET(SD_PORT, SD_CS);
  // Enable SPI, Set as Master
  //Prescaler: Fosc/16, Enable Interrupts
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0); // | (1 << SPR1)

  for (uint8_t i=0; i<10; i++){
    spi_send_(0xFF);
  }

  card_command_(CMD0, 0, 0x95);
  for (uint16_t retry = 0; ;retry++){
  if (retry == 0xFFFF) {
      return 0;
    }
    if (SPDR == R1_IDLE_STATE){
      break;
    }
    spi_send_(0xFF);
  }

  card_command_(CMD8, 0x01AA, 0x87);
  if (SPDR != 1){
    return 0;
  }
 
  for (uint16_t retry = 0; ; retry++) {
    card_command_(CMD55, 0, 0xFF);
    card_command_(ACMD41, 0x40000000, 0xFF);
    if (SPDR == R1_READY_STATE)break;
    if (retry == 1000) {
      return 0;
    }
  }

  card_command_(CMD58, 0, 0xFF );
  if(SPDR){
      return 0;
  }

  for (uint8_t i = 0; i < 4; i++){
    spi_send_(0xFF);
  }
  
  //use max SPI frequency
  SPCR &= ~((1 << SPR1) | (1 << SPR0)); // f_OSC/4
  SPSR |= (1 << SPI2X); // Doubled Clock Frequency: f_OSC/2
  SD_UNSET(SD_PORT, SD_CS);
  return 1;
}

static inline void spi_send_(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

static inline void card_command_(uint8_t cmd, uint32_t arg, uint8_t crc){
  // end read if in partialBlockRead mode
  spi_send_(0xFF);
  //select card
  SD_SET(SD_PORT, SD_CS);
  // send command
  spi_send_(cmd | 0x40);
  //send argument
  for (int8_t s = 24; s >= 0; s -= 8){
    spi_send_(arg >> s);
  }
  //send CRC
  spi_send_(crc);
  //wait for not busy
  spi_send_(0xFF);
  for (uint8_t retry = 0; retry < 0xFF; retry++){
    spi_send_(0xFF);
    if (SPDR != 0xFF){
      break;
    }
  }
}

static inline uint8_t wait_start_block_(void){
  uint16_t retry = 10000;
  do{
    spi_send_(0xFF);
    if (SPDR == DATA_START_BLOCK) return 1;
  }while(retry--);
  return 0;
}

static inline uint8_t read_sector_(uint32_t sector){
  card_command_(CMD17, sector, 0xFF);
  if (SPDR || !wait_start_block_()){
    return 0;
  }

  // read sector
  for (uint16_t i = 0; i < 512; i++) {
    spi_send_(0xFF);
    sector_buffer[i] = SPDR;
  }

  //read 2 bytes CRC (not used)
  spi_send_(0xFF);
  spi_send_(0xFF);
  SD_UNSET(SD_PORT, SD_CS);
  return 1;
}
