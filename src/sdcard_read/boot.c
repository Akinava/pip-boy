#include "boot.h"


int main(void){
  // setup button
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);

  /*
  // MAIL LOGIC
  while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    load(BOOT_APP);
  }else{
    goto (void*) 0;
  }
  */

  // FIXME DEBUG
  load(BOOT_APP);
  while(1){
  }

  return 0;
}

void load(const char* file_path){
  setup_led();

  if (!sd_init()){
    error_light();
  }

  displayBegin();
  displayClean();

  file_t boot_file;
  if (!file_open(file_path, &boot_file)){
    error_light();
    //error_blink();
  }
 
  //show_u32(boot_file.sector, 0, 0);
  //show_u32(boot_file.size, 0, 1);
  //displayUpdate();

  //fileOpen();
  //uint8_t data = fileRead();

  // FIXME
  //reboot();
 }


void setup_led(void){
  // setup led
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
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

void reboot(){
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = WATCHDOG_16MS;
	while (1);
}
