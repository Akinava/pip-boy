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
  if (event == A_KEY_PRESSED){cursor++;}
  if (event == C_KEY_PRESSED && cursor < 2){cursor--;}

  if (cursor == 2){
    print("              ", 0, 4);
    if (format()){
      print("format done    ", 0, 3);
    }else{
      print("format error   ", 0, 3);
    }
    return;
  }

  if (event == C_KEY_PRESSED && (cursor < 0 || cursor > 2)){
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

  memset(sector_buffer, 0, 512);
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
  uint16_t OEM_NAME_OFFSET = 0x03;
  uint8_t OEM_NAME[] = {'t', 'a', 'f', '.', 's', 'f', 'k', 'm'};

  uint16_t BYTE_PER_SECTOR_OFFSET = 0x0b;
  uint8_t BYTE_PER_SECTOR[] = {0x02, 0x00};

  uint16_t SECTORS_PER_CLUSTER_FAT16_OFFSET = 0x0d;
  uint8_t SECTORS_PER_CLUSTER_FAT16[] = {0x40};

  uint16_t RESERVED_SECTORS_FAT16_OFFSET = 0x0e;
  uint8_t RESERVED_SECTORS_FAT16[] = {0x00, 0x01};

  uint16_t FAT16_TABLES_OFFSET = 0x10;
  uint8_t FAT16_TABLES[] = {0x02};

  uint16_t ROOT_DIR_ENTITY_OFFSET = 0x11;
  uint8_t ROOT_DIR_ENTITY[] = {0x04, 0x00};

  uint16_t MEDIA_DESCRIPTOR_OFFSET = 0x15;
  uint8_t MEDIA_DESCRIPTOR[] = {0xf8};

  uint16_t SECTORS_PER_FAT16_OffSET = 0x16;
  uint8_t SECTORS_PER_FAT16[] = {0x01, 0x00};

  uint16_t PARTITION1_SIZE_OFFSET = 0x20;
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

  memset(sector_buffer, 0, 512);
 
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
      sector_buffer+SECTORS_PER_CLUSTER_FAT16_OFFSET,
      SECTORS_PER_CLUSTER_FAT16,
      sizeof(SECTORS_PER_CLUSTER_FAT16)
  );
  copy_data(
      sector_buffer+RESERVED_SECTORS_FAT16_OFFSET,
      RESERVED_SECTORS_FAT16,
      sizeof(RESERVED_SECTORS_FAT16)
  );
  copy_data(
      sector_buffer+FAT16_TABLES_OFFSET,
      FAT16_TABLES,
      sizeof(FAT16_TABLES)
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
      sector_buffer+PARTITION1_SIZE_OFFSET,
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

  write_sector_(PART1_START_SECTOR);

  memset(sector_buffer, 0, 512);
  
  // fat table
  uint16_t FAT16_FIRST_RECORD_OFFSET = 0;
  uint8_t FAT16_FIRST_RECORD[] = {0xff, 0xff, 0xff, 0xf8};

  
  for (uint8_t fat_tab=0; fat_tab<FAT16_TABLES_DEF; fat_tab++){
    copy_data(
        sector_buffer+FAT16_FIRST_RECORD_OFFSET,
        FAT16_FIRST_RECORD,
        sizeof(FAT16_FIRST_RECORD)
    );

    uint32_t cluster = fat_tab*SECTORS_PER_FAT16_TABLE +
      PART1_START_SECTOR +
      RESERVED_SECTORS_BEFORE_FAT16_TABLE;
    write_sector_(cluster);

    memset(sector_buffer, 0, 512);
    cluster++;

    for (uint16_t fat16_epmty_sectors=0; fat16_epmty_sectors<SECTORS_PER_FAT16_TABLE-1; fat16_epmty_sectors++){
      write_sector_(cluster+fat16_epmty_sectors);    
    }
  }

}

void fat32(void){

}

void copy_data(uint8_t* dst, uint8_t* src, uint16_t length){
  for (uint16_t i=0; i<length; i++){
    *(dst+i) = *(src+length-i-1);
  }
}

void get_part2_size(uint8_t* mbr_part2_size){
  
  uint32_t part2_size = card_size - PART2_START_SECTOR;
  for(uint8_t i=0; i<4; i++){
    mbr_part2_size[i] = part2_size >> 8*(3-i) & 0xff;
  }
}
