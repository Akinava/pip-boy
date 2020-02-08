#include "boot.h"


int main(void){
  // setup led
  SET_DDR_OUT(LED_DDR, LED_PIN);
  // setup button
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
  SET_LOW(LED_PORT, LED_PIN);

  // FIXME
  while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    // if button c is pressed load boot
    SET_HIGH(LED_PORT, LED_PIN);
  }// else goto 0 address


  if (!sd_init()){
    error_light();
  }

  uint32_t bin_dir_sector;
  if(!sd_find_obj(0, BOOT_DIR_NAME, &bin_dir_sector)){
    error_blink();
  }

  //fileOpen();
  //uint8_t data = fileRead();
  //fileRead();
  //fileClose();
  //sdStop();

  // FIXME 
  while(1){
     //SET_HIGH(LED_PORT, LED_PIN);
  }
  return 0;
}


void error_light(void){
    SET_HIGH(LED_PORT, LED_PIN);
    while(1){}
}

void error_blink(void){
  while(1){
    TOGGLE(LED_PORT, LED_PIN);
    _delay_ms(1000);
  }
}
