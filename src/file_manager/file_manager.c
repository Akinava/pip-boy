#include "file_manager.h"

int main(void){
  displayBegin();
  keys_setup();

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

void read_keyboard(void){
  // save from contact bounce
  _delay_ms(150);
  while(1){
    if(CHECK_PIN(BUTTON_UP_PINS, BUTTON_UP_PIN)){
      if (cursor == 0){continue;}
      cursor--;
      return;
    }
    if(CHECK_PIN(BUTTON_DOWN_PINS, BUTTON_DOWN_PIN)){
      if (cursor == list_lines-1){continue;}
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
