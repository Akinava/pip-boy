#include <avr/io.h>

#ifndef PINS_H                                            
#define PINS_H
                                                                                
// DEFINE BUTTON C                                                                 
#define BUTTON_C_DDR DDRC                                                         
#define BUTTON_C_PORT PORTC                                                       
#define BUTTON_C_PINS PINC                                                        
#define BUTTON_C_PIN PC3                                                     

// DEFINE LED
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD5

// DISPLAY
#define DISPLAY_DDR DDRC
#define DISPLAY_PORT PORTC
#define SDA PC4
#define SCL PC5

// SD
#define SDCS PB2

#endif
