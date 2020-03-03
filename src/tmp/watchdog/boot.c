#include "boot.h"

int main(void){
  setup_button_();
  setup_led_();
  
  //displayBegin();
  //displayClean();

  wdt_reset();
  wdt_disable(); 
  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    //wdt_enable(WDTO_1S);

    SET_HIGH(LED_PORT, LED_PIN);
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN));
  }
  SET_HIGH(LED_PORT, LED_PIN);
  wdt_enable(WDTO_120MS);
  while (1);

}

void setup_button_(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

void setup_led_(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}
