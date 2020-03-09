#include <avr/pgmspace.h>
#include <string.h>
#include "macro.h"
#include "pins.h"

#ifndef DISPLAY_H
#define DISPLAY_H

void setpos(void);

void displayBegin(void);
void displayClean(void);
void displayUpdate(void);
void print(char* str, uint8_t x, uint8_t y);
void print_invert(char* str, uint8_t x, uint8_t y);

#define SSD1306_ADDR                              0x3C
#define SSD1306_SET_START_LINE                    0x40
#define SSD1306_SET_SEGMENT_REMAP                 0xA0
#define SSD1306_COMMAND                           0x00
#define SSD1306_SET_COLUMN_ADDR                   0x21
#define SSD1306_SET_PAGE_ADDR                     0x22
#define SSD1306_DATA_CONTINUE                     0x40
#define TWI_FREQ 400000L
#define __cbi2(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#define FONT_OFFSET 32
#define FONT_WIDTH 5
#define FONT_HEIGHT 8

void _initTWI(void);
void _sendTWIcommand(uint8_t value); 
void _send_TWI_command_start(uint8_t command);
void print_string_(char* str, uint8_t x, uint8_t y);
void print_char_(uint8_t c, uint8_t buff_offset);
void display_update_(uint8_t y);

uint8_t buff_[127];
uint8_t invert_text_;
#endif
