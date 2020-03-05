#include <avr/io.h>

// DDR Data Direction Register                                                  
#define SET_DDR_OUT(DDR, PIN) DDR |= _BV(PIN)                                   
#define SET_DDR_IN(DDR, PIN) DDR &= ~ _BV(PIN)                                  
#define SET_PULLUP(PORTS, PIN) PORTS |= _BV(PIN)                                
                                                                                 
// PORTS Data Register                                                          
#define SET_HIGH(PORTS, PIN) PORTS |= _BV(PIN)                                  
#define SET_LOW(PORTS, PIN) PORTS &= ~ _BV(PIN)                                 
#define TOGGLE(PORTS, PIN) PORTS ^= _BV(PIN);                                   
                                                                               
// PINS Input Pins Register                                                     
#define CHECK_PIN(PINS, PIN) !(PINS & _BV(PIN))                                 
                                                                                
// DEFINE BUTTON C                                                                 
#define BUTTON_C_DDR DDRC                                                         
#define BUTTON_C_PORT PORTC                                                       
#define BUTTON_C_PINS PINC                                                        
#define BUTTON_C_PIN PC3                                                     

// DEFINE LED
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD5

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

  while(1){

    if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
      SET_LOW(LED_PORT, LED_PIN);
    }else{
      SET_HIGH(LED_PORT, LED_PIN);
    }
  }
}
