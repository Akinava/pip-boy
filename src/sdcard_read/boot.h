#include <avr/boot.h>
#include <util/delay.h>
#include <avr/io.h>
#include "macro.h"
#include "pins.h"
#include "sd_spi/sd.h"
                                                                               
#ifndef BOOT_H                                         
#define BOOT_H

const char BOOT_APP[] PROGMEM = "/BIN/BOOT.BIN";
void* start_address = 0;

uint8_t PAGE_BUFFER[SPM_PAGESIZE];
uint32_t ADDRESS;

void load(const char* file_path);
void block_flash_load();
void setup_button(void);
void setup_led(void);
void error_light(void);
void error_blink(void);
void reboot(void);

#define WATCHDOG_16MS   (_BV(WDE))

#endif
