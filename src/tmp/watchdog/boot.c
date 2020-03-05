#include "boot.h"
#include <avr/wdt.h>

#define WATCHDOG_RESET  (_BV(WDIE) | _BV(WDP2) | _BV(WDP1))
//#define WATCHDOG_OFF    (0)                                                       
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
//#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))

const int (*app_start)(void) = 0x0;

void setup_button_(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

void setup_led_(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}

void watchdog_config_(uint8_t x) {
  WDTCSR = _BV(WDCE) | _BV(WDE);                                                   
  WDTCSR = x;                                                                      
}

void restart_(void){
  watchdog_config_(WATCHDOG_125MS);
  while (1);
}

int main(void){
  setup_button_();
  /*
  //asm("cli");
  //asm volatile ("clr __zero_reg__");
  // off
  //watchdog_config_(WATCHDOG_OFF);
  // FIXME
  // cli();
  watchdog_config_(WATCHDOG_RESET);
  // sei();
  setup_led_();
  SET_HIGH(LED_PORT, LED_PIN);
  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN));
  }
  restart_();
  */
  //cli();
  //SP=RAMEND-9; // This is done by hardware reset
  //if (MCUSR & _BV(EXTRF)) {
  //  MCUSR = ~(_BV(WDRF));
  //}
  MCUSR &= ~(1<<WDRF);
  watchdog_config_(WATCHDOG_RESET);
  //sei();

  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    setup_led_();
    SET_HIGH(LED_PORT, LED_PIN);
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN));
    SET_LOW(LED_PORT, LED_PIN);
    _delay_ms(300);

    // load app

    restart_();
 }
  app_start();
}
