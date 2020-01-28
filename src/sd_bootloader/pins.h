#ifndef PINS_H                                                                  
#define PINS_H                                                                  

// LED
#define LED_PIN PD5
#define LED_DDR DDRD
#define LED_PORT PORTD

// BUTTON
#define BUTTON_PIN PC3
#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC
#define BUTTON_PINS PINC

// SD                                                                              
#define SD_DDR DDRB                                                                
#define SD_PORT PORTB                                                              
#define SD_CS PB2                                                                  
#define MOSI PB3                                                                   
#define MISO PB4                                                                   
#define SCK PB5
                                                                                
#endif 
