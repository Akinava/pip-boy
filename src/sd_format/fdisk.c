#include "fdisk.h"

int main(void){
  display_begin();
  author();
  display_clean();

  if (!card_init_()){
    print("SD read fail", 26, 3);
    while(1);
  }
  
  if (!get_card_size(&card_size)){
    print("can't read sd size", 0, 0);
    while(1);
  }
  print("card size:", 0, 0);
  print("unrecognized", 0, 1);
  for (uint8_t i=0; i<3; i++){
    if (card_size == size_ref[i].sise){
      clean_buf();
      print(size_ref[i].name, 0, 1);
    }
  }
  print("format sd card?", 0, 3);

  setup_keys();
  cursor = 0;
  event = NOOP;

  format_menu();
}

void format_menu(void){
  while(1){
    display_page();
    read_key();
    react();
  }
}

void display_page(void){
  if (cursor == 0){
    print("press A button", 0, 4);
  }
  if (cursor == 1){
    print_invert("press A button", 0, 4);
  }
}

void react(void){
  if (cursor < 2){
    if (event == A_KEY_PRESSED){cursor++;}
    if (event == C_KEY_PRESSED){cursor--;}
  }
 
  if (cursor == 2){
    print("               ", 0, 3);
    print("              ", 0, 4);
    if (format()){
      print("format done", 0, 3);
    }else{
      print("format error", 0, 3);
    }
    cursor = 4;
    return;
  }
  
  if (event == C_KEY_PRESSED && cursor < 0){
    app_exit();
  }
  if (event == C_KEY_PRESSED && cursor > 2){
    app_exit();
  }
}


void setup_keys(void){
  SET_DDR_IN(BUTTON_A_DDR, BUTTON_A_PIN);
  SET_PULLUP(BUTTON_A_PORT, BUTTON_A_PIN);
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

void read_key(void){
  event = NOOP; 
  while(event == NOOP){
    if(CHECK_PIN(BUTTON_A_PINS, BUTTON_A_PIN)){event = A_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){event = C_KEY_PRESSED;}
    VOLUME_ID += 1;
  }
  // for the contact bounce
  _delay_ms(150);
}

void app_exit(void){
  display_clean();
  print("DON'T TURN OFF!", 0, 0);
  load_default_app();
}

void author(void){
  display_clean();
  print("   Andrei Chernov", 0, 2);
  print("  akinava@gmail.com", 0, 4);
  print("        2020", 0, 6);
  _delay_ms(2000);
  display_clean();
}

uint8_t format(void){
  mbr();
  fat16();
  fat32();
  return 1; 
}

void mbr(void){
  memset(sector_buffer, 0, 512);
  // MBR
  uint16_t MBR_BOOT_LOADER_CODE_SIZE = 0x1be;
  uint8_t MBR_VOLUME_RECORD_SIZE = 0x10;

  uint8_t MBR_VOLUME_TYPE_OFFSET = 0x04;
  uint8_t MBR_VOLUME_TYPE_FAT16[] = {0x06};
  uint8_t MBR_VOLUME_TYPE_FAT32[] = {0x0b};

  uint8_t MBR_START_SECTOR_OFFSET = 0x08;
  uint8_t MBR_PART1_START_SECTOR[] = {0x00, 0x00, 0x00, 0x01};
  uint8_t MBR_PART2_START_SECTOR[] = {0x00, 0x3f, 0xff, 0x42};

  uint8_t MBR_PART_SIZE_OFFSET = 0x0c;
  uint8_t MBR_PART1_SIZE[] = {0x00, 0x3f, 0xff, 0x41};

  uint16_t buffer_offset = MBR_BOOT_LOADER_CODE_SIZE;

  // FAT16
  copy_data(
      sector_buffer+buffer_offset+MBR_VOLUME_TYPE_OFFSET,
      MBR_VOLUME_TYPE_FAT16,
      sizeof(MBR_VOLUME_TYPE_FAT16)
  );
  copy_data(
      sector_buffer+buffer_offset+MBR_START_SECTOR_OFFSET,
      MBR_PART1_START_SECTOR,
      sizeof(MBR_PART1_START_SECTOR)
  );
  copy_data(
      sector_buffer+buffer_offset+MBR_PART_SIZE_OFFSET,
      MBR_PART1_SIZE,
      sizeof(MBR_PART1_SIZE)
  );
  buffer_offset += MBR_VOLUME_RECORD_SIZE;

  // FAT32
  copy_data(
      sector_buffer+buffer_offset+MBR_VOLUME_TYPE_OFFSET,
      MBR_VOLUME_TYPE_FAT32,
      sizeof(MBR_VOLUME_TYPE_FAT32)
  );
  copy_data(
      sector_buffer+buffer_offset+MBR_START_SECTOR_OFFSET,
      MBR_PART2_START_SECTOR,
      sizeof(MBR_PART2_START_SECTOR)
  );
  uint8_t mbr_part2_size[4];
  get_part2_size(mbr_part2_size);
  copy_data(
      sector_buffer+buffer_offset+MBR_PART_SIZE_OFFSET,
      mbr_part2_size,
      sizeof(mbr_part2_size)
  );

  // END OF BLOCK
  copy_data(
      sector_buffer+END_OF_BLOCK_OFFSET,
      END_OF_BLOCK,
      sizeof(END_OF_BLOCK)
  );

  write_sector_(0); 
}

void fat16(void){
  memset(sector_buffer, 0, 512);

  uint8_t SECTORS_PER_CLUSTER_FAT16[] = {0x40};

  uint16_t RESERVED_SECTORS_FAT16_OFFSET = 0x0e;
  uint8_t RESERVED_SECTORS_FAT16[] = {0x00, 0x01};

  uint16_t ROOT_DIR_ENTITY_OFFSET = 0x11;
  uint8_t ROOT_DIR_ENTITY[] = {0x04, 0x00};

  uint16_t SECTORS_PER_FAT16_OffSET = 0x16;
  uint8_t SECTORS_PER_FAT16[] = {0x01, 0x00};

  uint8_t PARTITION1_SIZE[] = {0x00, 0x3f, 0xff, 0x41};

  uint16_t DRIVE_NUMBER_OFFSET = 0x24;
  uint8_t DRIVE_NUMBER[] = {0x80};

  uint16_t RESERVED_FAT16_OFFSET = 0x25;
  uint8_t RESERVED_FAT16[] = {0x01};

  uint16_t BOOT_SIGNATURE_OFFSET = 0x26;
  uint8_t BOOT_SIGNATURE[] = {0x29};

  uint16_t VOLUME1_ID_OFFSET = 0x27;
  uint8_t VOLUME1_ID[4];
  for (uint8_t i=0; i<4; i++){
    VOLUME1_ID[i] = VOLUME_ID>>(8*i)&0xff;
  }

  uint16_t VOLUME_FAT16_LABLE_OFFSET = 0x2b;
  uint8_t VOLUME_FAT16_LABLE[] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'I', 'B'};

  uint16_t FAT16_SYSTEM_NAME_OFFSET = 0x36;
  uint8_t FAT16_SYSTEM_NAME[] = {' ', ' ', ' ', '6', '1', 'T', 'A', 'F'};

  copy_data(
      sector_buffer+OEM_NAME_OFFSET,
      OEM_NAME,
      sizeof(OEM_NAME)
  );
  copy_data(
      sector_buffer+BYTE_PER_SECTOR_OFFSET,
      BYTE_PER_SECTOR,
      sizeof(BYTE_PER_SECTOR)
  );
  copy_data(
      sector_buffer+SECTORS_PER_CLUSTER_OFFSET,
      SECTORS_PER_CLUSTER_FAT16,
      sizeof(SECTORS_PER_CLUSTER_FAT16)
  );
  copy_data(
      sector_buffer+RESERVED_SECTORS_FAT16_OFFSET,
      RESERVED_SECTORS_FAT16,
      sizeof(RESERVED_SECTORS_FAT16)
  );
  copy_data(
      sector_buffer+FAT_TABLES_OFFSET,
      FAT_TABLES,
      sizeof(FAT_TABLES)
  );
  copy_data(
      sector_buffer+ROOT_DIR_ENTITY_OFFSET,
      ROOT_DIR_ENTITY,
      sizeof(ROOT_DIR_ENTITY)
  );
  copy_data(
      sector_buffer+MEDIA_DESCRIPTOR_OFFSET,
      MEDIA_DESCRIPTOR,
      sizeof(MEDIA_DESCRIPTOR)
  );
  copy_data(
      sector_buffer+SECTORS_PER_FAT16_OffSET,
      SECTORS_PER_FAT16,
      sizeof(SECTORS_PER_FAT16)
  );
  copy_data(
      sector_buffer+PARTITION_SIZE_OFFSET,
      PARTITION1_SIZE,
      sizeof(PARTITION1_SIZE)
  );
  copy_data(
      sector_buffer+DRIVE_NUMBER_OFFSET,
      DRIVE_NUMBER,
      sizeof(DRIVE_NUMBER)
  );
  copy_data(
      sector_buffer+RESERVED_FAT16_OFFSET,
      RESERVED_FAT16,
      sizeof(RESERVED_FAT16)
  );
  copy_data(
      sector_buffer+BOOT_SIGNATURE_OFFSET,
      BOOT_SIGNATURE,
      sizeof(BOOT_SIGNATURE)
  );
  copy_data(
      sector_buffer+VOLUME1_ID_OFFSET,
      VOLUME1_ID,
      sizeof(VOLUME1_ID)
  );
  copy_data(
      sector_buffer+VOLUME_FAT16_LABLE_OFFSET,
      VOLUME_FAT16_LABLE,
      sizeof(VOLUME_FAT16_LABLE)
  );
  copy_data(
      sector_buffer+FAT16_SYSTEM_NAME_OFFSET,
      FAT16_SYSTEM_NAME,
      sizeof(FAT16_SYSTEM_NAME)
  );

  // END OF BLOCK
  copy_data(
      sector_buffer+END_OF_BLOCK_OFFSET,
      END_OF_BLOCK,
      sizeof(END_OF_BLOCK)
  );

  // write pbp fat16
  write_sector_(PART1_START_SECTOR);

  memset(sector_buffer, 0, 512);
  
  // fat16 table
  uint16_t FAT16_FIRST_RECORD_OFFSET = 0;
  uint8_t FAT16_FIRST_RECORD[] = {0xff, 0xff, 0xff, 0xf8};
  copy_data(
    sector_buffer+FAT16_FIRST_RECORD_OFFSET,
    FAT16_FIRST_RECORD,
    sizeof(FAT16_FIRST_RECORD)
  );
  
  // write fat16 table
  uint32_t fat16_table_sector = PART1_START_SECTOR + 
                                RESERVED_SECTORS_BEFORE_FAT16_TABLE;
  write_sector_(fat16_table_sector);

  // write fat16 table copy
  uint32_t fat16_table_sector_copy = fat16_table_sector + 
                                     SECTORS_PER_FAT16_TABLE;
  write_sector_(fat16_table_sector_copy);

  // write root dir label
  memset(sector_buffer, 0, 512);
  copy_data(
      sector_buffer,
      VOLUME_FAT16_LABLE,
      sizeof(VOLUME_FAT16_LABLE)
  );
  copy_data(
      sector_buffer+sizeof(VOLUME_FAT16_LABLE),
      ROOT_LABLE_FLAG,
      sizeof(ROOT_LABLE_FLAG)
  );

  uint32_t root_sector = 2 +
    // fat tables
    2 * SECTORS_PER_FAT16_TABLE;
  write_sector_(root_sector);

  // write fat16 tables empty sectors
  memset(sector_buffer, 0, 512);

  print("step 5-0", 30, 3);
  write_blocks(
      fat16_table_sector+1,
      SECTORS_PER_FAT16_TABLE-1);

  print("step 5-1", 30, 3);
  write_blocks(fat16_table_sector_copy+1,
    SECTORS_PER_FAT16_TABLE-1);

  // write root dir
  uint16_t root_sectors = int_from_array(
      ROOT_DIR_ENTITY,
      sizeof(ROOT_DIR_ENTITY)); 

  print("step 5-2", 30, 3);
  write_blocks(root_sector+1, root_sectors-1);
}

void fat32(void){
  memset(sector_buffer, 0, 512);
  // FAT32 BRB
  uint8_t SECTORS_PER_CLUSTER_FAT32[] = {0x01};

  uint16_t RESERVED_SECTORS_FAT32_OFFSET = 0xe;
  uint8_t RESERVED_SECTORS_FAT32[] = {0x00, 0x20};
  uint8_t RESERVED_SECTORS_FAT32_VOLUME = 0x20;

  uint16_t SECTORS_PER_FAT32_OffSET = 0x24;

  uint16_t ROOT_DIR_CLUSTER_OFFSET = 0x2C;
  uint8_t ROOT_DIR_CLUSTER[] = {0x00, 0x00, 0x00, 0x02};

  uint16_t SF_INFO_SECTOR_OFFSET = 0x30;
  uint8_t SF_INFO_SECTOR[] = {0x00, 0x01};

  uint16_t FAT32_BOOT_COPY_SECTOR_OFFSET = 0x32;
  uint8_t FAT32_BOOT_COPY_SECTOR[] = {0x00, 0x06};
  uint8_t FAT32_BOOT_COPY_SECTOR_VOLUME = 6;

  uint16_t VOLUME_FAT32_LABLE_OFFSET = 0x47;
  uint8_t VOLUME_FAT32_LABLE[] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', 'T', 'A', 'D'};

  uint16_t FAT32_SYSTEM_NAME_OFFSET = 0x52;
  uint8_t FAT32_SYSTEM_NAME[] = {' ', ' ', ' ', '2', '3', 'T', 'A', 'F'};

  copy_data(
      sector_buffer+OEM_NAME_OFFSET,
      OEM_NAME,
      sizeof(OEM_NAME)
  );
  copy_data(
      sector_buffer+BYTE_PER_SECTOR_OFFSET,
      BYTE_PER_SECTOR,
      sizeof(BYTE_PER_SECTOR)
  );
  copy_data(
      sector_buffer+SECTORS_PER_CLUSTER_OFFSET,
      SECTORS_PER_CLUSTER_FAT32,
      sizeof(SECTORS_PER_CLUSTER_FAT32)
  );
  copy_data(
      sector_buffer+RESERVED_SECTORS_FAT32_OFFSET,
      RESERVED_SECTORS_FAT32,
      sizeof(RESERVED_SECTORS_FAT32)
  );
  copy_data(
      sector_buffer+FAT_TABLES_OFFSET,
      FAT_TABLES,
      sizeof(FAT_TABLES)
  );
  copy_data(
      sector_buffer+MEDIA_DESCRIPTOR_OFFSET,
      MEDIA_DESCRIPTOR,
      sizeof(MEDIA_DESCRIPTOR)
  );
  uint8_t part2_size[4];
  get_part2_size(part2_size);
  copy_data(
      sector_buffer+PARTITION_SIZE_OFFSET,
      part2_size,
      sizeof(part2_size)
  );
  uint8_t sectors_per_fat32[4];
  get_sectors_per_fat32(sectors_per_fat32);
  copy_data(
      sector_buffer+SECTORS_PER_FAT32_OffSET,
      sectors_per_fat32,
      sizeof(sectors_per_fat32)
  );
  copy_data(
      sector_buffer+ROOT_DIR_CLUSTER_OFFSET,
      ROOT_DIR_CLUSTER,
      sizeof(ROOT_DIR_CLUSTER)
  );
  copy_data(
      sector_buffer+SF_INFO_SECTOR_OFFSET,
      SF_INFO_SECTOR,
      sizeof(SF_INFO_SECTOR)
  );
  copy_data(
      sector_buffer+FAT32_BOOT_COPY_SECTOR_OFFSET,
      FAT32_BOOT_COPY_SECTOR,
      sizeof(FAT32_BOOT_COPY_SECTOR)
  );
  copy_data(
      sector_buffer+VOLUME_FAT32_LABLE_OFFSET,
      VOLUME_FAT32_LABLE,
      sizeof(VOLUME_FAT32_LABLE)
  );
  copy_data(
      sector_buffer+FAT32_SYSTEM_NAME_OFFSET,
      FAT32_SYSTEM_NAME,
      sizeof(FAT32_SYSTEM_NAME)
  );
      
  // END OF BLOCK
  copy_data(
      sector_buffer+END_OF_BLOCK_OFFSET,
      END_OF_BLOCK,
      sizeof(END_OF_BLOCK)
  );

  // write BPB
  write_sector_(PART2_START_SECTOR);
  // write BPB copy
  write_sector_(PART2_START_SECTOR+FAT32_BOOT_COPY_SECTOR_VOLUME);

  memset(sector_buffer, 0, 512);
  // FAT32 tables
  uint8_t FAT32_0[] = {0x0f, 0xff, 0xff, 0xf8};
  uint8_t FAT32_1[] = {0x0f, 0xff, 0xff, 0xff};
  uint16_t cluster_offset = 0;
 
  copy_data(
      sector_buffer,
      FAT32_0,
      sizeof(FAT32_0)
  );
  cluster_offset += sizeof(FAT32_0);

  copy_data(
      sector_buffer+cluster_offset,
      FAT32_1,
      sizeof(FAT32_1)
  );
  cluster_offset += sizeof(FAT32_1);

  copy_data(
      sector_buffer+cluster_offset,
      FAT32_0,
      sizeof(FAT32_0)
  );

  // write fat table
  uint32_t fat32_table_sector = PART2_START_SECTOR+
                                RESERVED_SECTORS_FAT32_VOLUME;
  write_sector_(fat32_table_sector);

  // write fat copy table
  uint32_t fat32_table_sector_copy = fat32_table_sector+
                                     sectors_per_fat32_volue;
  write_sector_(fat32_table_sector_copy);

  // write root dir lable
  memset(sector_buffer, 0, 512);
  copy_data(
      sector_buffer,
      VOLUME_FAT32_LABLE,
      sizeof(VOLUME_FAT32_LABLE)
  );
  copy_data(
      sector_buffer+sizeof(VOLUME_FAT32_LABLE),
      ROOT_LABLE_FLAG,
      sizeof(ROOT_LABLE_FLAG)
  );
  uint32_t root_sector = fat32_table_sector_copy+
    sectors_per_fat32_volue;
  write_sector_(root_sector);


  // write fat32 tables empty sectors
  memset(sector_buffer, 0, 512);
  print("step 5-3", 30, 3);
  write_blocks(fat32_table_sector+1,
    sectors_per_fat32_volue-1);

  print("step 5-4", 30, 3);
  write_blocks(fat32_table_sector_copy+1,
    sectors_per_fat32_volue-1);

  // wrire root dir empty sectors
  uint16_t root_secrors = int_from_array(
      SECTORS_PER_CLUSTER_FAT32,
      sizeof(SECTORS_PER_CLUSTER_FAT32));

  print("step 5-5", 30, 3);
  write_blocks(root_sector+1,
    root_secrors-1);
  print("", 0, 5);
  print("", 0, 6);
}

void copy_data(uint8_t* dst, uint8_t* src, uint16_t length){
  for (uint16_t i=0; i<length; i++){
    *(dst+i) = *(src+length-i-1);
  }
}

void get_part2_size(uint8_t* part2_size){
  
  uint32_t part2_size_volue = card_size - PART2_START_SECTOR;
  for(uint8_t i=0; i<4; i++){
    part2_size[i] = part2_size_volue >> 8*(3-i) & 0xff;
  }
}

void get_sectors_per_fat32(uint8_t* sectors_per_fat32){
  sectors_per_fat32_volue =  (
      card_size - 
      PART2_START_SECTOR - 
      0x18) / 130;

  for(uint8_t i=0; i<4; i++){
    sectors_per_fat32[i] = sectors_per_fat32_volue >> 8*(3-i) & 0xff;
  }
}

void write_blocks(uint32_t start_sector, uint32_t sectors){
  clean_buf();
  print32(sectors, 30, 5);

  for (uint32_t i=0; i<sectors; i++){
    if (i%0xff == 0){print32(i, 30, 6);}
    if (sector_is_empty(start_sector+i)){continue;}
    write_sector_(start_sector+i);  
  }
}

uint32_t int_from_array(uint8_t* arr, uint8_t len){
  uint32_t res = 0;
  for (uint16_t i=0; i<len; i++){
    res += arr[i] << 8 * (len-i-1);
  }
  return res;
}

uint8_t sector_is_empty(uint32_t sector){
  uint8_t sector_empty_flag = 1;
  card_command_(CMD17, sector, 0xFF);
  if (SPDR || !wait_start_block_()){
    return 0;
  }

  // read sector
  for (uint16_t i = 0; i < SECTOR_SIZE; i++) {
    spi_send_(0xFF);
    if (SPDR != 0){
      sector_empty_flag = 0;
    }
  }

  //read 2 bytes CRC (not used)
  spi_send_(0xFF);
  spi_send_(0xFF);
  SD_UNSET(SD_PORT, SD_CS);

  return sector_empty_flag;
}
 
