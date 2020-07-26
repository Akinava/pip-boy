#include "sd.h"

uint8_t choose_file_menu(uint16_t* app_file_cluster, uint32_t* app_file_size, char* file_name_buf){
  display_clean();
  if (!sd_init()){
    print("SD read fail", 26, 3);
    _delay_ms(1000);
    return 0;
  }

  sd_menu.cursor = 0;
  sd_menu.max_lines = LINES;
  app_chosen = 0;

  while(!app_chosen){
    if (make_list()){
      jump_cursor();
    }else{
      step_cursor_back();
    }

    while (check_cursor_in_page()){
      show_page();
      if (read_keyboard()){
        break;
      }
    }
  }

  // copy claster and size
  obj_data_t obj = objects_data[sd_menu.cursor];
  *app_file_cluster = obj.data_cluster;
  *app_file_size = obj.size;
  // copy name
  memset(file_name_buf, 0, 1+8+1+3);
  compose_obj_name(obj, file_name_buf);
  return 1;
}

void select_app(void){
  app_chosen = 1;
}

void show_page(void){
  display_clean();
  //       dir_flag  name  dot ext  0x0
  char buf[1+        8+    1+  3+   1];
  for (uint8_t y=0; y<sd_menu.lines; y++){
    copy_line_(buf, y);
    if (y == sd_menu.cursor){
      print_invert(buf, 0, y);
    }else{
      print(buf, 0, y);
    }
  }
}

uint8_t read_keyboard(void){
  uint8_t event = read_key(); 
  if (event == UP_KEY_PRESSED){
      sd_menu.cursor--;
      return 0;
  }
  if (event == DOWN_KEY_PRESSED){
      sd_menu.cursor++;
      return 0;
  }
  if (event == A_KEY_PRESSED){
      select_obj();
  }
  if (event == C_KEY_PRESSED){
      menu.cursor = 0;
  }
  return 1;
}

uint8_t make_list(void){
  return read_directory_page(&sd_menu, objects_data);
}

void jump_cursor(void){
  if (sd_menu.cursor == -1){sd_menu.cursor = sd_menu.max_lines -1;}
  if (sd_menu.cursor == sd_menu.max_lines){sd_menu.cursor = 0;}
}

void step_cursor_back(){
  if (sd_menu.cursor == -1){
    sd_menu.cursor = 0;
    return;
  }
  sd_menu.cursor -= 1;
}

uint8_t check_cursor_in_page(void){
  if (check_jump_to_next_page()){
    return 0;
  }

  // if lines < max_lines it means this is the last page
  if (sd_menu.cursor == sd_menu.lines){
    step_cursor_back();
  }
  return 1;
}

uint8_t check_jump_to_next_page(void){
  if (sd_menu.cursor == -1){
    sd_menu.next_page = ABOVE;
    return 1;
  }
  if (sd_menu.cursor == sd_menu.max_lines){
    sd_menu.next_page = BELOW;
    return 1;
  }
  return 0;
}

void set_dirirectory_as_current(void){
  obj_data_t obj = objects_data[sd_menu.cursor];
  vol_info.primary_dir_cluster = obj.data_cluster;
  sd_menu.cursor = 0;
}

void select_obj(void){
  if (objects_data[sd_menu.cursor].dir == FILE_FLAG){
    select_app();
  }else{
    set_dirirectory_as_current();
  }
}

void copy_line_(char* buf, uint8_t y){
  memset(buf, ' ', 1+8+1+3);
  *(buf+1+8+1+3) = 0;
  // add "*" symbol to makr a dir
  if (objects_data[y].dir){
    *(buf) = '*';
  }else{
    *(buf) = ' ';
  }

  compose_obj_name(objects_data[y], buf+1);
}

uint8_t compose_obj_name(obj_data_t obj, char* buff_dst){
  uint8_t length = 0;
  // search end of obj name and copy
  while (obj.name[length] != ' ' && length < 8){
    buff_dst[length] = obj.name[length];
    length++;
  }

  // copy ext
  if (obj.dir){return length;}
  // add dot
  buff_dst[length++] = '.';

  // add ext
  
  uint8_t i = 8;
  while((i < OBJECT_NAME_SIZE) && (obj.name[i] != ' ')){
    buff_dst[length++] = obj.name[i++];
  }
  return length;
}

/*************************** choose file menu ********************************/

void cp_obj_name_(char* dst, uint16_t buffer_offset){
  for (uint8_t i=0; i< OBJECT_NAME_SIZE; i++){
    *(dst+i) = *(sector_buffer+buffer_offset+i);
  }
}

uint8_t copy_obj_to_page_(obj_data_t* objects_data, obj_data_t* src_obj, sd_menu_t* sd_menu){
  if (*(sector_buffer+src_obj->sector_offset) == OBJECT_IS_DELETED){
    return 0;
  }
  uint8_t src_object_index = sd_menu->lines;
  if (sd_menu->next_page == ABOVE){
    src_object_index = sd_menu->max_lines - sd_menu->lines - 1;
  }
  obj_data_t* dst_obj = objects_data + src_object_index;
  dst_obj->cluster = src_obj->cluster;
  dst_obj->sector = src_obj->sector;
  dst_obj->dir = FILE_FLAG;
  if (*(sector_buffer+src_obj->sector_offset+OBJ_ATTRIBUTES_OFFSET) == OBJ_CATALOG){
    dst_obj->dir = DIR_FLAG;
  }
  dst_obj->sector_offset = src_obj->sector_offset;
  dst_obj->data_cluster = *((uint16_t*)(sector_buffer+src_obj->sector_offset+DATA_CLUSTER_OFFSET));
  dst_obj->size = *((uint32_t*)(sector_buffer+src_obj->sector_offset+OBJ_SIZE_OFFSET)); 
  cp_obj_name_(dst_obj->name, src_obj->sector_offset);
  return 1;
}

uint32_t get_root_last_sector(){
  /*                            /------------------------  0x400 * 32 / 0x200 = 64 ----------------------------\   */
  return vol_info.root_sector + vol_info.root_directory_entries * OBJECT_RECORD_SIZE / vol_info.bytes_per_sector; 
}

uint8_t next_cluster_by_fat_(obj_data_t* prev_object, obj_data_t* next_object){                              
  uint16_t fat_cluster_place = prev_object->cluster*FAT_CLUSTER_SIZE;
  uint16_t fat_sector_offset = fat_cluster_place / vol_info.bytes_per_sector; 
  if (!read_sector_(vol_info.fat_table_sector+fat_sector_offset)){return 0;}                                    
  next_object->cluster = *((uint32_t*)(sector_buffer + fat_cluster_place));            
  if (next_object->sector >= END_OF_CLUSTERCHAIN){return 0;}                           
  return 1;                                                                     
} 

uint8_t prev_cluster_by_fat_(obj_data_t* prev_object, obj_data_t* next_object){
  if (prev_object->cluster == vol_info.primary_dir_cluster){return 0;}
  obj_data_t tmp_obj;
  tmp_obj.cluster = vol_info.primary_dir_cluster;
  *next_object = tmp_obj;
  while(tmp_obj.cluster != prev_object->cluster){
    *next_object = tmp_obj;
    //                   prev         next
    if (!next_cluster_by_fat_(next_object, &tmp_obj)){return 0;}
  }
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
      next_object->cluster = prev_object->sector + 1;
      return 1;
    }
  }else{
    // data cluster
    /* current sector of cluster - first sector of cluster + 1           */
    if (prev_object->sector-get_sector_by_cluster_(prev_object->cluster)+1 < vol_info.sectors_per_cluster){
      next_object->cluster = prev_object->cluster + 1;
      return 1;
    }else{
      if (next_cluster_by_fat_(prev_object, next_object)){
        return 1;
      }
    }
  }
  return 0;
}

uint8_t prev_cluster_(obj_data_t* prev_object, obj_data_t* next_object){
  // check overflow offset
  if (prev_object->sector_offset != 0){
    next_object->cluster = prev_object->cluster;
    return 1;
  }
  
  if (prev_object->cluster == ROOT_CLUSTER){
    // check overflow sectors in rood directory
    if (prev_object->sector > vol_info.root_sector){
      next_object->cluster = prev_object->sector - 1;
      return 1;
    }
  }else{
    // check overflow sectors in data directory
    /*  current sector of cluster | first sector of cluster               */
    if (prev_object->sector > get_sector_by_cluster_(prev_object->cluster)){
      next_object->cluster = prev_object->cluster - 1;
      return 1;
    }else{
      if (prev_cluster_by_fat_(prev_object, next_object)){
        return 1;
      }
    }
  }
  // this is the first cluster
  return 0;
}

void next_sector_(obj_data_t* prev_object, obj_data_t* next_object){
  if (prev_object->sector_offset+OBJECT_RECORD_SIZE < vol_info.bytes_per_sector){
    next_object->sector = prev_object->sector;
  }else{
    next_object->sector = prev_object->sector + 1;
  }
}

void prev_sector_(obj_data_t* prev_object, obj_data_t* next_object){
  if (prev_object->sector_offset != 0){
    next_object->sector = prev_object->sector;
  }else{
    next_object->sector = prev_object->sector - 1;
  }
}

void next_sector_offset_(obj_data_t* prev_object, obj_data_t* next_object){
  if (prev_object->sector_offset+OBJECT_RECORD_SIZE < vol_info.bytes_per_sector){
    next_object->sector_offset = prev_object->sector_offset + OBJECT_RECORD_SIZE;
  }else{
    next_object->sector_offset = 0;  
  }
}

void prev_sector_offset_(obj_data_t* prev_object, obj_data_t* next_object){
  if (prev_object->sector_offset == 0){
    next_object->sector_offset = vol_info.bytes_per_sector - OBJECT_RECORD_SIZE;
  }else{
    next_object->sector_offset = prev_object->sector_offset - OBJECT_RECORD_SIZE;
  }
}

void get_prev_obj(obj_data_t* objects_data, obj_data_t* prev_obj, sd_menu_t* sd_menu){
  if (sd_menu->cursor == sd_menu->max_lines){
    *prev_obj = objects_data[sd_menu->max_lines-1];
    return;
  }
  *prev_obj = objects_data[0];
}

uint8_t get_next_obj_(obj_data_t* prev_obj, obj_data_t* next_obj, sd_menu_t* sd_menu){
  // ABOVE PAGE
  if (sd_menu->next_page == ABOVE){
    if (!prev_cluster_(prev_obj, next_obj)){return 0;}
    prev_sector_(prev_obj, next_obj);
    prev_sector_offset_(prev_obj, next_obj);
    return 1;
  }
  // BELOW PAGE
  if (!next_cluster_(prev_obj, next_obj)){return 0;}
  next_sector_(prev_obj, next_obj);
  next_sector_offset_(prev_obj, next_obj);
  return 1;
 }

void get_zero_obj_(obj_data_t* next_obj){
  next_obj->cluster = vol_info.primary_dir_cluster;
  next_obj->sector = get_sector_by_cluster_(next_obj->cluster);
  next_obj->sector_offset = 0;
}

uint8_t check_object_exist(obj_data_t obj){
  if (sector_buffer[obj.sector_offset] == OBJECT_NOT_EXIST){
    return 0;
  }
  return 1;
}

uint8_t read_directory_page(sd_menu_t* sd_menu, obj_data_t* objects_data){
  obj_data_t prev_obj;
  obj_data_t next_obj;
  get_prev_obj(objects_data, &prev_obj, sd_menu);

  if (sd_menu->cursor == 0){
    sd_menu->next_page = BELOW;
    // cluster saved in first object
    get_zero_obj_(&next_obj);
  }else{
    if (!get_next_obj_(&prev_obj, &next_obj, sd_menu)){
      return 0;  
    }
  }
  if (!read_sector_(next_obj.sector)){return 0;}

  sd_menu->lines = 0;
  while (sd_menu->lines < sd_menu->max_lines){
    if (!check_object_exist(next_obj)){break;}
    if (copy_obj_to_page_(objects_data, &next_obj, sd_menu)){(sd_menu->lines)++;}
  
    prev_obj = next_obj;
    if (!get_next_obj_(&prev_obj, &next_obj, sd_menu)){break;}
    // if the same secror we do not need read it again
    if (prev_obj.sector != next_obj.sector){
      if (!read_sector_(next_obj.sector)){break;}
    }
  }

  if (sd_menu->cursor != 0 && sd_menu->lines == 0){
    sd_menu->lines = sd_menu->max_lines;
    return 0;
  }
  return 1;
}

uint32_t get_sector_by_cluster_(uint16_t cluster){
  if (cluster == ROOT_CLUSTER){
    return vol_info.root_sector;
  }
  return vol_info.data_sector + ((cluster-2) * vol_info.sectors_per_cluster);
}

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
  vol_info.primary_dir_cluster = ROOT_CLUSTER; 
  return 1;
}

uint8_t card_init_(void){
  // off master PIN and radio
  RADIO_DDR |= _BV(RADIO_CSN)|_BV(SPI_MASTER_PIN);
  SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  SPI_UNSET(RADIO_PORT, RADIO_CSN);

  // Set MOSI, SCK, CS as Output
  SD_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // set sd cs off
  SD_UNSET(SD_PORT, SD_CS);
  // Enable SPI, Set as Master
  //Prescaler: Fosc/16, Enable Interrupts
  //SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0); // | (1 << SPR1)
  //Prescaler: Fosc/4
  SPCR = _BV(SPE)|_BV(MSTR);

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
    if (SPDR == R1_READY_STATE){break;}
    if (retry == 1000){return 0;}
  }

  card_command_(CMD58, 0, 0xFF );
  if(SPDR){return 0;}

  for (uint8_t i = 0; i < 4; i++){
    spi_send_(0xFF);
  }
  
  //use max SPI frequency
  //SPCR &= ~((1 << SPR1) | (1 << SPR0)); // f_OSC/4
  //SPSR |= (1 << SPI2X); // Doubled Clock Frequency: f_OSC/2
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
  }while(retry--);
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
