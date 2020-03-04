#include "boot.h"

#define WATCHDOG_OFF    (0)                                                        
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))

void setup_button_(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

void setup_led_(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}

void watchdog_config(uint8_t x) {                                                   
  WDTCSR = _BV(WDCE) | _BV(WDE);                                                   
  WDTCSR = x;                                                                      
}

int main(void){
  setup_button_();
  setup_led_();

  if (MCUSR & _BV(EXTRF)) {
    MCUSR = ~(_BV(WDRF));
  }

  watchdog_config(WATCHDOG_OFF);

  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    SET_HIGH(LED_PORT, LED_PIN);
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN));
  }
  SET_HIGH(LED_PORT, LED_PIN);

  watchdog_config(WATCHDOG_125MS);
  while (1);
}
