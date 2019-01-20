#define F_CPU 16000000UL // 16 MHz
 
#include <avr/io.h>
#include <util/delay.h>


// DDR Data Direction Register
#define SET_DDR_OUT(DDR, PIN) DDR |= _BV(PIN)
#define SET_DDR_IN(DDR, PIN) DDR &= ~ _BV(PIN)

// PORTS Data Register 
#define SET_HIGH(PORTS, PIN) PORTS |= _BV(PIN)
#define SET_LOW(PORTS, PIN) PORTS &= ~ _BV(PIN)
#define TOGGLE(PORTS, PIN) PORTS ^= _BV(PIN);
#define SET_PULLUP(PORTS, PIN) PORTS |= _BV(PIN)


// PINS Input Pins Register
//#define SET_PULLUP(PINS, PIN) PINS &= _BV(PIN)
#define CHECK_PIN(PINS, PIN) !(PINS & _BV(PIN))

// DEFINE PINS
#define LED_PIN PD5
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON_PIN PC3
#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC
#define BUTTON_PINS PINC


void start_blink(void);

int main(void){
 
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_DDR_IN(BUTTON_DDR, BUTTON_PIN);
  //SET_PULLUP(BUTTON_PINS, BUTTON_PIN);
  SET_PULLUP(BUTTON_PORT, BUTTON_PIN);

  start_blink();

  while(1){

    if (CHECK_PIN(BUTTON_PINS, BUTTON_PIN)){
      void* bl = (void *) 0x7e00;
      goto *bl;
     }else{
      SET_HIGH(LED_PORT, LED_PIN);
    } 
  }
  return 0;
}

void start_blink(void){
  for (uint8_t i=0; i<4; i++){
    TOGGLE(LED_PORT, LED_PIN);
    _delay_ms (500);
  }
}
