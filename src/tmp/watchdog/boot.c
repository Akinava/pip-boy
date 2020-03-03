#include "boot.h"

void setup_button_(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

void setup_led_(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}

int main(void){
  setup_button_();
  setup_led_();

  // WATCHDOG_OFF
  //WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;

  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    SET_HIGH(LED_PORT, LED_PIN);
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN));
  }
  SET_HIGH(LED_PORT, LED_PIN);

  // WATCHDOG_125MS
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDP1) | _BV(WDP0) | _BV(WDE);

  while (1);
}
