#include <util/delay.h>
#include "macro.h"
#include "pins.h"
#include "display_oled_i2c/display.h"
#include "sd_spi/sd.h"

#ifndef BOOT_H                                         
#define BOOT_H
                                                                                
const char BOOT_NAME[] PROGMEM = "boot.bin";
uint8_t PAGE_BUFFER[SPM_PAGESIZE];

#endif
