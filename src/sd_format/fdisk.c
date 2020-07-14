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
  memset(sector_buffer, 0, 512);
  mbr();
  fat16();
  fat32();
  return 1; 
}

void mbr(void){
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

}

void fat32(void){

}

void copy_data(uint8_t* dst, const uint8_t* src, uint16_t length){
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
