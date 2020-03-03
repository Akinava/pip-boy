#include "boot.h"

void setup_button_(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

void setup_led_(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}

void watchdogConfig(uint8_t x) {
  //WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = x;
}

int main(void){
  setup_button_();
  setup_led_();

  // MCUSR ??
  /*
  displayBegin();
  displayClean();
  //WDTCSR    // 0
  //_BV(WDP2) | _BV(WDP1) | _BV(WDE) // 0x0e 1s
  //_BV(WDP1) | _BV(WDP0) | _BV(WDE) // 0x0b 125ms
  //WDCE      // 4
  //WDE       // 3

  displayUpdate();
  while(1);
  */
  
  
  //wdt_reset();
  //wdt_disable(); 
  watchdogConfig(0); // WATCHDOG_OFF
  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    //wdt_enable(WDTO_1S);
    watchdogConfig(_BV(WDP2) | _BV(WDP1) | _BV(WDE)); // WATCHDOG_1S

    SET_HIGH(LED_PORT, LED_PIN);
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN));
  }
  SET_HIGH(LED_PORT, LED_PIN);
  watchdogConfig(0xb); // WATCHDOG_125MS
  //wdt_enable(WDTO_120MS);
  while (1);
}
