#include "menu.h"

void menu_init(void){
  setup_keys();
}

void setup_keys(void){
  SET_DDR_IN(BUTTON_A_DDR, BUTTON_A_PIN);
  SET_PULLUP(BUTTON_A_PORT, BUTTON_A_PIN);
  SET_DDR_IN(BUTTON_B_DDR, BUTTON_B_PIN);
  SET_PULLUP(BUTTON_B_PORT, BUTTON_B_PIN);
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
  SET_DDR_IN(BUTTON_UP_DDR, BUTTON_UP_PIN);
  SET_PULLUP(BUTTON_UP_PORT, BUTTON_UP_PIN);
  SET_DDR_IN(BUTTON_DOWN_DDR, BUTTON_DOWN_PIN);
  SET_PULLUP(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
  SET_DDR_IN(BUTTON_LEFT_DDR, BUTTON_LEFT_PIN);
  SET_PULLUP(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
  SET_DDR_IN(BUTTON_RIGHT_DDR, BUTTON_RIGHT_PIN);
  SET_PULLUP(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN);
}

uint8_t read_key(void){
  uint8_t event = NOOP; 
  while(!event){
    if(CHECK_PIN(BUTTON_A_PINS, BUTTON_A_PIN)){event = A_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_B_PINS, BUTTON_B_PIN)){event = B_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){event = C_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_UP_PINS, BUTTON_UP_PIN)){event = UP_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_DOWN_PINS, BUTTON_DOWN_PIN)){event = DOWN_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_RIGHT_PINS, BUTTON_RIGHT_PIN)){event = RIGHT_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_LEFT_PINS, BUTTON_LEFT_PIN)){event = LEFT_KEY_PRESSED;}
  }
  // for the contact bounce
  _delay_ms(150);
  return event;
}