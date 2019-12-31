#include <avr/pgmspace.h>
#include "../macro.h"
#include "../pins.h"

#ifndef DISPLAY_H
#define DISPLAY_H


void displayBegin(void);
void displayClean(void);
void displayUpdate(void);
void displayPrintHex(uint8_t value);



#define SSD1306_ADDR                              0x3C
#define SSD1306_DISPLAY_OFF                       0xAE
#define SSD1306_DISPLAY_ON                        0xAF
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO       0xD5
#define SSD1306_SET_MULTIPLEX_RATIO               0xA8
#define SSD1306_SET_DISPLAY_OFFSET                0xD3
#define SSD1306_SET_START_LINE                    0x40
#define SSD1306_SET_COM_PINS                      0xDA
#define SSD1306_CHARGE_PUMP                       0x8D
#define SSD1306_MEMORY_ADDR_MODE                  0x20
#define SSD1306_SET_SEGMENT_REMAP                 0xA0
#define SSD1306_COM_SCAN_DIR_DEC                  0xC8
#define SSD1306_SET_CONTRAST_CONTROL              0x81
#define SSD1306_SET_PRECHARGE_PERIOD              0xD9
#define SSD1306_SET_VCOM_DESELECT                 0xDB
#define SSD1306_DISPLAY_ALL_ON_RESUME             0xA4
#define SSD1306_NORMAL_DISPLAY                    0xA6
#define SSD1306_COMMAND                           0x00
#define TWI_FREQ 400000L


#define __cbi2(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))


void _initTWI(void);
void _sendTWIcommand(uint8_t value); 


#endif
