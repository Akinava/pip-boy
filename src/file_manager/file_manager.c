#include "file_manager.h"

int main(void){
  display_begin();
  display_clean();

  if (!sd_init()){
    print("SD read fail", 26, 3);
    while(1);
  }

  /*
  print32(vol_info.start_sector, 0, 0);      // 0x0800
  print32(vol_info.fat_table_sector, 0, 1);  // 0x0800+0x0400=0x0840
  print32(vol_info.root_sector, 0, 2);       // 0x0840+0x0100*0x02=0x0a40
  print32(vol_info.data_sector, 0, 3);       // 0x0a40+0x0400*32/0x0200=0x0a80
  */

  keys_setup();

  cursor = 0;
  parents_cluster = 0;
  objects_data[0].sector = vol_info.root_sector;
  objects_data[0].sector_offset = 0;

  while(1){
    if(make_list()){
      show_list();}
    read_keyboard();
  }
}

uint8_t make_list(void){
  lines = read_dir(LINES, objects_data, cursor);
  return 1;
}

void show_list(void){
  if (cursor == -1 || cursor == LINES){cursor = 0;}
  display_clean();
  char buf[1+8+1+3+1];
  for (uint8_t y=0; y<lines; y++){
    copy_line_(buf, y);
    if (y == cursor){
      print_invert(buf, 0, y);
    }else{
      print(buf, 0, y);
    }
  }
}

void read_keyboard(void){
  // avoid from contact bounce
  _delay_ms(150);
  while(1){
    if(CHECK_PIN(BUTTON_UP_PINS, BUTTON_UP_PIN)){
      cursor--;
      return;
    }
    if(CHECK_PIN(BUTTON_DOWN_PINS, BUTTON_DOWN_PIN)){
      cursor++;
      return;
    }
  }
}

void keys_setup(void){
  SET_DDR_IN(BUTTON_A_DDR, BUTTON_A_PIN);
  SET_PULLUP(BUTTON_A_PORT, BUTTON_A_PIN);

  SET_DDR_IN(BUTTON_UP_DDR, BUTTON_UP_PIN);
  SET_PULLUP(BUTTON_UP_PORT, BUTTON_UP_PIN);

  SET_DDR_IN(BUTTON_DOWN_DDR, BUTTON_DOWN_PIN);
  SET_PULLUP(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
}

void copy_line_(char* buf, uint8_t y){
  memset(buf, ' ', 1+8+1+3);
  *(buf+1+8+1+3) = 0;
  // mark * for dir
  if (objects_data[y].dir){
    *(buf) = '*';
  }else{
    *(buf) = ' ';
  }
  // search end of obj name
  uint8_t end_of_obj_name;
  uint8_t i;
  for(i=7; i>=0; i--){
    if (objects_data[y].name[i] != ' '){
      end_of_obj_name = i;
      break;
    }
  }
  // copy name
  for (i=0; i<=end_of_obj_name; i++){
    *(buf+i+1) = objects_data[y].name[i];
  }
  // copy ext
  if (objects_data[y].dir){return;}
  // dot
  *(buf+end_of_obj_name+2) = '.';
  // ext
  uint8_t ext_cour = end_of_obj_name+3;
  for (i=0; i<3; i++){
    *(buf+ext_cour+i) = objects_data[y].name[8+i];
  }

}
