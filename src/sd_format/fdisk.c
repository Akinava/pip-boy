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
  format();
  print("press A button", 0, 4);
}

void format(void){
  while(1){
    display_page();
    read_key();
    react();
  }
}

void display_page(){
  if (cursor == 0){
    print("press A button", 0, 4);
  }
  if (cursor == 1){
    print_invert("press A button", 0, 4);
  }
}

void author(void){
  display_clean();
  print("   Andrei Chernov", 0, 2);
  print("  akinava@gmail.com", 0, 4);
  print("        2020", 0, 6);
  _delay_ms(2000);
  display_clean();
}

