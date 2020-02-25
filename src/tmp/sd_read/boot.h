#include <avr/boot.h>
#include <util/delay.h>
#include <avr/io.h>
#include "macro.h"
#include "pins.h"
#include "sd.h"

#include "display.h"
                                                                               
#ifndef BOOT_H                                         
#define BOOT_H

const char BOOT_APP[] PROGMEM = "/BIN/BOOT.BIN";
const int (*app_start)(void) = 0x0;

void load(const char* file_path);
void block_flash_load_(uint32_t* address);
void setup_button_(void);
void setup_led_(void);
void error_light_(void);
void error_blink_(void);
void reboot_(void);

#endif
