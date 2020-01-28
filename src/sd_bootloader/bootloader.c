#include "bootloader.h"


int main(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_DDR_IN(BUTTON_DDR, BUTTON_PIN);
  SET_PULLUP(BUTTON_PORT, BUTTON_PIN);

  if (CHECK_PIN(BUTTON_PINS, BUTTON_PIN)){
    load(default_program);
  }
  goto *(void*) 0;
  return 0;
}


void load(const char * filename){

}


void error(void){
  while (1){
    TOGGLE(LED_PORT, LED_PIN);
    _delay_ms (300);
  }
}
