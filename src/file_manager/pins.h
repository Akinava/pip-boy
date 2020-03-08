#ifndef PINS_H                                            
#define PINS_H
                                                                                
// DEFINE BUTTON A
#define BUTTON_A_DDR DDRD
#define BUTTON_A_PORT PORTD
#define BUTTON_A_PINS PIND
#define BUTTON_A_PIN PD4

// DEFINE BUTTON C
#define BUTTON_C_DDR DDRC
#define BUTTON_C_PORT PORTC
#define BUTTON_C_PINS PINC
#define BUTTON_C_PIN PC3

// DEFINE BUTTON UP
#define BUTTON_UP_DDR DDRB
#define BUTTON_UP_PORT PORTB
#define BUTTON_UP_PINS PINB
#define BUTTON_UP_PIN PB1

// DEFINE BUTTON DOWN
#define BUTTON_DOWN_DDR DDRD
#define BUTTON_DOWN_PORT PORTD
#define BUTTON_DOWN_PINS PIND
#define BUTTON_DOWN_PIN PD6

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
#define SD_DDR DDRB
#define SD_PORT PORTB
#define SD_CS PB2
#define MOSI PB3
#define MISO PB4
#define SCK PB5

#endif
