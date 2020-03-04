#include <avr/io.h>

// DDR Data Direction Register
#define SET_DDR_OUT(DDR, PIN) DDR |= _BV(PIN)
#define SET_DDR_IN(DDR, PIN) DDR &= ~ _BV(PIN)

// PORTS Data Register 
#define SET_HIGH(PORTS, PIN) PORTS |= _BV(PIN)
#define SET_LOW(PORTS, PIN) PORTS &= ~ _BV(PIN)
#define TOGGLE(PORTS, PIN) PORTS ^= _BV(PIN);
#define SET_PULLUP(PORTS, PIN) PORTS |= _BV(PIN)

// PINS Input Pins Register
#define CHECK_PIN(PINS, PIN) !(PINS & _BV(PIN))

// DEFINE PINS
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD5


int main(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_HIGH(LED_PORT, LED_PIN);
  while(1);
}
