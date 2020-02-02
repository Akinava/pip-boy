#include <avr/pgmspace.h> 
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/io.h>                                                                
#include <util/delay.h>


#ifndef BOOT_H                                            
#define BOOT_H
                                                                                
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

// DEFINE PIN                                                                  
#define BUTTON_DDR DDRC                                                         
#define BUTTON_PORT PORTC                                                       
#define BUTTON_PINS PINC                                                        
#define BUTTON_PIN PC3                                                     

// DEFINE LED PIN
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD5


#define BOOT_ADDRESS 0x2000
const char BOOT_NAME[] PROGMEM = "boot.bin";
static void blockFlashLoad(uint16_t size);
uint8_t pageBuffer[SPM_PAGESIZE];
uint16_t address;

#endif
