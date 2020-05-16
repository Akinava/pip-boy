#include <avr/boot.h>
#include <util/delay.h>
#include <avr/io.h>
#include "macro.h"
#include "pins.h"
#include "sd.h"

#ifndef BOOT_H                                         
#define BOOT_H

#define WATCHDOG_RESET  (_BV(WDIE) | _BV(WDP2) | _BV(WDP1))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))

const char BOOT_APP[] PROGMEM = "/BIN/FM.BIN";
const int (*app_start)(void) = 0x0;

void load_default_app(void);
void load_app_by_name(const char* file_path);
void load_app_by_cluster(uint16_t cluster, uint32_t size);
void block_flash_load_(uint32_t* address, uint8_t page);
void setup_button_(void);
void setup_led_(void);
void error_light_(void);
void error_blink_(void);
void watchdog_config_(uint8_t x);

#endif
