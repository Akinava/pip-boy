#include "file_manager.h"

int main(void){
  displayBegin();

  while(1){
    make_list();
    show_list();
    read_keyboard();
  }
}

void make_list(void){
  strcpy(list_buf[0],"Volume 0");
  strcpy(list_buf[1],"Volume 1");
  strcpy(list_buf[2],"Volume 2");
  strcpy(list_buf[3],"Volume 3");
  list_lines = 4;
}

void show_list(void){
  displayClean();
  for (uint8_t y=0; y<list_lines; y++){
    if (y == cursor){
      print_invert(list_buf[y], 0, y*8);
    }else{
      print(list_buf[y], 0, y*8);
    }
  }
  displayUpdate();
}

void read_keyboard(){
  _delay_ms(50);
}
