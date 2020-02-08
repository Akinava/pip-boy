#include <util/delay.h>
#include "macro.h"
#include "pins.h"
#include "display_oled_i2c/display.h"
#include "sd_spi/sd.h"

#ifndef BOOT_H                                         
#define BOOT_H
                                                                                
const char BOOT_FILE_NAME[] PROGMEM = "BOOT    BIN";
const char BOOT_DIR_NAME[] PROGMEM = "BIN        ";
uint8_t PAGE_BUFFER[SPM_PAGESIZE];

void error_light(void);
void error_blink(void);

#endif
