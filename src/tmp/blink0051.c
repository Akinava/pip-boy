#define F_CPU 16000000UL // 16 MHz
 
#include <avr/io.h>
#include <util/delay.h>


// DDR Data Direction Register
#define SET_DDR_OUT(DDR, PIN) DDR |= _BV(PIN)

// PORTS Data Register 
#define SET_HIGH(PORTS, PIN) PORTS |= _BV(PIN)                                  
#define SET_LOW(PORTS, PIN) PORTS &= ~ _BV(PIN)                                 

// DEFINE PINS
#define LED_PIN PD5
#define LED_DDR DDRD
#define LED_PORT PORTD

int main(void){
 
  SET_DDR_OUT(LED_DDR, LED_PIN);
  while(1){
    SET_HIGH(LED_PORT, LED_PIN);
    _delay_ms(50);
    SET_LOW(LED_PORT, LED_PIN);
    _delay_ms(1000);
  }
  return 0;
}
