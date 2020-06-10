#include <avr/io.h>

#ifndef PINS_H
#define PINS_H

// DEFINE BUTTON A
#define BUTTON_A_DDR DDRD
#define BUTTON_A_PORT PORTD
#define BUTTON_A_PINS PIND
#define BUTTON_A_PIN PD4

// DEFINE BUTTON B
#define BUTTON_B_DDR DDRD
#define BUTTON_B_PORT PORTD
#define BUTTON_B_PINS PIND
#define BUTTON_B_PIN PD2

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

// DEFINE BUTTON LEFT
#define BUTTON_LEFT_DDR DDRB
#define BUTTON_LEFT_PORT PORTB
#define BUTTON_LEFT_PINS PINB
#define BUTTON_LEFT_PIN PB0

// DEFINE BUTTON RIGHT
#define BUTTON_RIGHT_DDR DDRD
#define BUTTON_RIGHT_PORT PORTD
#define BUTTON_RIGHT_PINS PIND
#define BUTTON_RIGHT_PIN PD7

// DEFINE LED
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD5

// DEFINE VIBRO
#define VIBRO_DDR DDRC
#define VIBRO_PORT PORTC
#define VIBRO_PIN PC2

// DISPLAY
#define DISPLAY_DDR DDRC
#define DISPLAY_PORT PORTC
#define SDA PC4
#define SCL PC5

// SD
#define SD_DDR DDRB
#define SD_PORT PORTB
#define SD_CS PB2

// DARIO
#define RADIO_DDR DDRD
#define RADIO_PORT PORTD
#define RADIO_CE PD0
#define RADIO_CSN PD1

// SPI
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define MOSI PB3
#define MISO PB4
#define SCK PB5

// SPEAKER
#define SPEAKER_DDR DDRD
#define SPEAKER_PORT PORTD
#define SPEAKER_PIN PD3

#endif