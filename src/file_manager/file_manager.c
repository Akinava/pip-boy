#include "file_manager.h"

int main(void){
  display_begin();
  display_clean();

  if (!sd_init()){
    print("SD read fail", 26, 3);
    while(1);
  }

  setup_keys();

  menu.cursor = 0;
  menu.max_lines = LINES;

  while(1){
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
}

uint8_t make_list(void){
  return read_directory_page(&menu, objects_data);
}

void show_page(void){
  display_clean();
  //       dir_flag  name  dot ext  0x0
  char buf[1+        8+    1+  3+   1];
  for (uint8_t y=0; y<menu.lines; y++){
    copy_line_(buf, y);
    if (y == menu.cursor){
      print_invert(buf, 0, y);
    }else{
      print(buf, 0, y);
    }
  }
}

uint8_t check_jump_to_next_page(){
  if (menu.cursor == -1){
    menu.next_page = ABOVE;
    return 1;
  }
  if (menu.cursor == menu.max_lines){
    menu.next_page = BELOW;
    return 1;
  }
  return 0;
}

uint8_t check_cursor_in_page(){
  if (check_jump_to_next_page()){
    return 0;
  }

  // if lines < max_lines it means this is the last page
  if (menu.cursor == menu.lines){
    step_cursor_back();
  }
  return 1;
}

void step_cursor_back(){
  if (menu.cursor == -1){
    menu.cursor = 0;
    return;
  }
  menu.cursor -= 1;
}

void jump_cursor(void){
  if (menu.cursor == -1){menu.cursor = menu.max_lines -1;}
  if (menu.cursor == menu.max_lines){menu.cursor = 0;}
}

void set_dirirectory_as_current(void){
  obj_data_t obj = objects_data[menu.cursor];
  vol_info.primary_dir_cluster = obj.data_cluster;
  menu.cursor = 0;
}

void load_app(void){
  obj_data_t obj = objects_data[menu.cursor];
  // print name of app
  display_clean();
  print("load app", 26, 3);
  clean_buf();
  for (uint8_t i=0; i<8+3; i++){
    print_char_(obj.name[i], 8*i);
  }
  display_update_(4);

  load_app_by_cluster(obj.data_cluster, obj.size);
}

void select_obj(void){
  if (objects_data[menu.cursor].dir == FILE_FLAG){
    load_app();
  }else{
    set_dirirectory_as_current();
  }
}

uint8_t read_keyboard(void){
  // for the contact bounce
  _delay_ms(150);
  while(1){
    if(CHECK_PIN(BUTTON_UP_PINS, BUTTON_UP_PIN)){
      menu.cursor--;
      return 0;
    }
    if(CHECK_PIN(BUTTON_DOWN_PINS, BUTTON_DOWN_PIN)){
      menu.cursor++;
      return 0;
    }
    if(CHECK_PIN(BUTTON_A_PINS, BUTTON_A_PIN)){
      select_obj();
      break;
    }
    if(CHECK_PIN(BUTTON_C_PINS, BUTTON_A_PIN)){
      menu.cursor = 0;
      // goto parent dir;
      break;
    }
   }
  return 1;
}

void setup_keys(void){
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
