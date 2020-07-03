#include "fdisk.h"

int main(void){
  display_begin();
  author();
  display_clean();

  if (!card_init_()){
    print("SD read fail", 26, 3);
    while(1);
  }
  
  uint32_t card_size;
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
  if (event == C_KEY_PRESSED){cursor--;}
  if (cursor == 2){
    if (format()){
      print("format done", 0, 6);
    }else{
      print("format error", 0, 6);
    }
    cursor = 0;
  }
  if (cursor < 0){cursor = 0;}
  if (event == C_KEY_PRESSED && cursor == 0){
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
  memset(sector_buffer, 0x01, 512);
  write_sector_(0);
  return 1; 
}


