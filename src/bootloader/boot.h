#include <avr/boot.h>
#include <util/delay.h>
#include <avr/io.h>
#include "macro.h"
#include "pins.h"
#include "sd.h"
                                                                               
#ifndef BOOT_H                                         
#define BOOT_H

const char BOOT_APP[] PROGMEM = "/BIN/BOOT.BIN";
const void (*app_start)(void) = 0x0;

void load(const char* file_path);
static void block_flash_load();
static void setup_button(void);
static void setup_led(void);
static void error_light(void);
void error_blink(void);
static void reboot(void);

#endif
