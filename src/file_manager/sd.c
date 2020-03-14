#include "sd.h"

/*
uint8_t find_obj_by_name(file_t* file){
  // parameters:
  // obj_name - object name and ext in fat16 format cahr[8+3], exp: "APP     BIN"
  // file     -  file sector and file size
  uint8_t sectors = vol_info.root_directory_entries * OBJECT_RECORD_SIZE / vol_info.bytes_per_sector; 

  if (file->sector != root_sector_){
     sectors = vol_info.sectors_per_claster;
  }

  uint8_t records_per_sector = vol_info.bytes_per_sector / OBJECT_RECORD_SIZE;

  do{
    for (uint8_t sector_offset = 0; sector_offset < sectors; sector_offset++){
      if (!read_sector_(file->sector+sector_offset)){return 0;}

      for (uint8_t record_offset = 0; record_offset < records_per_sector; record_offset++){
        cp_record_data_(sector_buffer + record_offset * 32);

        if (check_obj_has_name_()){
          file_info_parce_(file, obj_data_);
          return 1;
        }
      }
    }
  }while(next_claster_(file));
  return 0;
}
*/

uint8_t read_dir(uint8_t count, obj_data_t* objects_data){
  objects_data[0].dir = 1;
  objects_data[0].cluster = 0;
  objects_data[0].sector = vol_info.root_sector;
  objects_data[0].sector_offset = 0;
  objects_data[0].data_cluster = 3;
  strcpy(objects_data[0].name, "BIN     ");

  objects_data[1].dir = 1;
  objects_data[1].cluster = 0;
  objects_data[1].sector = vol_info.root_sector;
  objects_data[1].sector_offset = 32;
  objects_data[1].data_cluster = 4;
  strcpy(objects_data[1].name, "SAVE    ");

  objects_data[2].dir = 1;
  objects_data[2].cluster = 0;
  objects_data[2].sector = vol_info.root_sector;
  objects_data[2].sector_offset = 64;
  objects_data[2].data_cluster = 5;
  strcpy(objects_data[2].name, "FONT    ");

  objects_data[3].dir = 0;
  objects_data[3].cluster = 0;
  objects_data[3].sector = vol_info.root_sector;
  objects_data[3].sector_offset = 98;
  objects_data[3].data_cluster = 6;
  objects_data[3].name[0] = 'B';
  objects_data[3].name[1] = 'O';
  objects_data[3].name[2] = 'O';
  objects_data[3].name[3] = 'T';
  objects_data[3].name[4] = ' ';
  objects_data[3].name[5] = ' ';
  objects_data[3].name[6] = ' ';
  objects_data[3].name[7] = ' ';
  objects_data[3].name[8] = 'B';
  objects_data[3].name[9] = 'I';
  objects_data[3].name[10] = 'N';
  return 4;
}

uint8_t next_claster_(obj_data_t* obj){
  /*
  // FIXME
  //if (obj->sector == vol_info.root_sector){return 0;}
  uint16_t fat_cluster_size = sizeof(file->cluster);
  uint16_t fat_cluster_place = file->cluster*fat_cluster_size;
  if (!read_sector_(vol_info.fat_table_sector)){return 0;}
  file->cluster = *((uint32_t*)(sector_buffer + fat_cluster_place));
  if (file->sector >= END_OF_CLASTERCHAIN){return 0;}
  get_sector_by_cluster_(file->cluster);
  */
  return 1;
}

/*
void file_info_parce_(file_t* file, uint8_t* file_info){
  file->cluster = *((uint16_t*)(file_info + FILE_CLUSTER_OFFSET));
  file->size = *((uint32_t*)(file_info + FILE_SIZE_OFFSET));
  get_sector_by_cluster_(file);
}
*/

uint32_t get_sector_by_cluster_(uint16_t cluster){
  return vol_info.data_sector + ((cluster-2) * vol_info.sectors_per_claster);
}

/*
void cp_record_data_(uint8_t* buffer){
  for (uint8_t i=0; i<OBJECT_RECORD_SIZE; i++){
    *(obj_data_+i) = *(buffer+i);
  }
}
*/

uint8_t sd_init(void){
  if (!card_init_()){return 0;}
  if (!vol_init_()){return 0;}
  return 1;
}

uint8_t vol_init_(void){
  // volume address
  if(!read_sector_(0)){return 0;}
  vol_info.start_sector = *((uint32_t*)(sector_buffer + VOL_ADDRESS_OFFSET));
  // volume info
  if(!read_sector_(vol_info.start_sector)){return 0;}
  vol_info = *((vol_info_t*)(sector_buffer + VOL_INFO_OFFSET));

  vol_info.fat_table_sector = vol_info.start_sector + vol_info.reserved_sectors; 
  vol_info.start_sector = vol_info.fat_table_sector + vol_info.sectors_per_FAT * vol_info.number_of_FATs;
  vol_info.data_sector = vol_info.start_sector + vol_info.root_directory_entries * OBJECT_RECORD_SIZE / vol_info.bytes_per_sector;
  return 1;
}

uint8_t card_init_(void){
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

void spi_send_(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

void card_command_(uint8_t cmd, uint32_t arg, uint8_t crc){
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

uint8_t wait_start_block_(void){
  uint16_t retry = 10000;
  do{
    spi_send_(0xFF);
    if (SPDR == DATA_START_BLOCK) return 1;
  }while(retry);
  return 0;
}

uint8_t read_sector_(uint32_t sector){
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
