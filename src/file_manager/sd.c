#include "sd.h"

void cp_obj_name_(char* dst, uint16_t buffer_offset){
  for (uint8_t i=0; i< OBJECT_NAME_SIZE; i++){
    *(dst+i) = *(sector_buffer+buffer_offset+i);
  }
}

uint8_t parsing_obj_data_(obj_data_t* obj, uint16_t buffer_offset){
  if (*(sector_buffer+buffer_offset) == OBJECT_IS_DELETED){
    return 0;
  }
  obj->dir = FILE_FLAG;
  if (*(sector_buffer+buffer_offset+OBJ_ATTRIBUTES_OFFSET) == OBJ_CATALOG){
    obj->dir = DIR_FLAG;
  }
  obj->sector_offset = buffer_offset;
  obj->data_cluster = *((uint16_t*)(sector_buffer+buffer_offset+DATA_CLUSTER_OFFSET));
  cp_obj_name_(obj->name, buffer_offset);
  return 1;
}

uint8_t find_read_direction(uint8_t count, int8_t cursor){
  if (cursor == -1){
    return READ_UP;
  }
  return READ_DOWN;
}

uint32_t get_root_last_sector(){
  /*                            /------------------------  0x400 * 32 / 0x200 = 64 ----------------------------\   */
  return vol_info.root_sector + vol_info.root_directory_entries * OBJECT_RECORD_SIZE / vol_info.bytes_per_sector; 
}

uint8_t next_cluster_by_fat_(obj_data_t* prev_object, obj_data_t* next_object){                              
  uint16_t fat_cluster_size = sizeof(prev_object->cluster);                            
  uint16_t fat_cluster_place = prev_object->cluster*fat_cluster_size;                  
  if (!read_sector_(vol_info.fat_table_sector)){return 0;}                                    
  next_object->cluster = *((uint32_t*)(sector_buffer + fat_cluster_place));            
  if (next_object->sector >= END_OF_CLUSTERCHAIN){return 0;}                           
  return 1;                                                                     
} 

uint8_t next_cluster_(obj_data_t* prev_object, obj_data_t* next_object){
  // check overflow offset
  if (prev_object->sector_offset+OBJECT_RECORD_SIZE < vol_info.bytes_per_sector){
    next_object->cluster = prev_object->cluster;
    return 1;
  }

  // check overflow sectors
  if (prev_object->cluster == ROOT_CLUSTER){
    // root cluster
    if (prev_object->sector+1 < get_root_last_sector()){
      next_object->cluster = prev_object->sector+1;
      return 1;
    }
  }else{
    // data cluster
    /* current sector of cluster - first sector of cluster + 1           */
    if (prev_object->sector-get_sector_by_cluster_(prev_object->cluster)+1 < vol_info.sectors_per_cluster){
      next_object->cluster = prev_object->cluster+1;
      return 1;
    }else{
      if (next_cluster_by_fat_(prev_object, next_object)){
        return 1;
      }
    }
  }
  return 0;
}

void next_sector_(obj_data_t* prev_object, obj_data_t* next_object){
  if (prev_object->sector_offset+OBJECT_RECORD_SIZE < vol_info.bytes_per_sector){
    next_object->sector = prev_object->sector;
  }else{
    next_object->sector = prev_object->sector+1;
  }
}

void next_sector_offset_(obj_data_t* prev_object, obj_data_t* next_object){
  if (prev_object->sector_offset+OBJECT_RECORD_SIZE < vol_info.bytes_per_sector){
    next_object->sector_offset = prev_object->sector_offset+OBJECT_RECORD_SIZE;
  }else{
    next_object->sector_offset = 0;  
  }
}

void clean_objects_data(uint8_t count, obj_data_t* objects_data){
  for (uint8_t i=0; i<count; i++){
    (objects_data+i)->dir = FILE_FLAG;
    (objects_data+i)->data_cluster = 0;
    (objects_data+i)->cluster = 0;
    (objects_data+i)->sector = 0;
    (objects_data+i)->sector_offset = 0;
    memset((objects_data+i)->name, ' ', OBJECT_NAME_SIZE);
  }
}

obj_data_t get_prev_obj(obj_data_t* objects_data, uint8_t cursor, uint8_t count){
  if (cursor == count){
    return *(objects_data+count-1);
  }
  return *(objects_data+count-1);
}

uint8_t get_next_obj_(obj_data_t* prev_obj, obj_data_t* next_obj, uint8_t read_direction){
  if (read_direction == READ_DOWN){
    if (!next_cluster_(prev_obj, next_obj)){
      return 0;
    }
    next_sector_(prev_obj, next_obj);
    next_sector_offset_(prev_obj, next_obj);
    return 1;
  }
  // FIXME READ UP
  return 0;

}

void get_zero_obj_(obj_data_t* next_obj){
  next_obj->cluster = ROOT_CLUSTER;
  next_obj->sector = vol_info.root_sector;
  next_obj->sector_offset = 0;
}

uint8_t read_dir(uint8_t count, obj_data_t* objects_data, int8_t cursor){
  obj_data_t prev_obj;
  obj_data_t next_obj;
  uint8_t read_direction = find_read_direction(count, cursor);
  prev_obj = get_prev_obj(objects_data, cursor, count);
 
  // init
  clean_objects_data(count, objects_data);
  uint8_t  item = 0;
  if (cursor == 0){
    get_zero_obj_(&next_obj);
  }else{
    if (!get_next_obj_(&prev_obj, &next_obj, read_direction)){
      return 0;  
    }
  }
  if (!read_sector_(next_obj.sector)){return 0;}

  while (item < count){
    if (parsing_obj_data_(&objects_data[item], next_obj.sector_offset)){item++;}
    prev_obj = next_obj;
    if (!get_next_obj_(&prev_obj, &next_obj, read_direction)){break;}
    // if the same secror we do not need read it again
    if (prev_obj.sector != next_obj.sector){
      if (!read_sector_(next_obj.sector)){break;}
    }
  }
  return item;
}

/*
void file_info_parce_(file_t* file, uint8_t* file_info){
  file->cluster = *((uint16_t*)(file_info + FILE_CLUSTER_OFFSET));
  file->size = *((uint32_t*)(file_info + FILE_SIZE_OFFSET));
  get_sector_by_cluster_(file);
}
*/

uint32_t get_sector_by_cluster_(uint16_t cluster){
  return vol_info.data_sector + ((cluster-2) * vol_info.sectors_per_cluster);
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
  uint32_t start_sector = *((uint32_t*)(sector_buffer + VOL_ADDRESS_OFFSET));

  // volume info
  if(!read_sector_(start_sector)){return 0;}
  vol_info = *((vol_info_t*)(sector_buffer + VOL_INFO_OFFSET));

  vol_info.start_sector = start_sector;
  vol_info.fat_table_sector = vol_info.start_sector + vol_info.reserved_sectors; 
  vol_info.root_sector = vol_info.fat_table_sector + vol_info.sectors_per_FAT * vol_info.number_of_FATs;
  vol_info.data_sector = vol_info.root_sector + vol_info.root_directory_entries * OBJECT_RECORD_SIZE / vol_info.bytes_per_sector;
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
