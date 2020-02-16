#include <util/delay.h>
#include "macro.h"
#include "pins.h"
#include "display_oled_i2c/display.h"
#include "sd_spi/sd.h"

#ifndef BOOT_H                                         
#define BOOT_H
                                                                                
const char BOOT_APP[] PROGMEM = "/BIN/BOOT.BIN";
//const char BOOT_APP[] PROGMEM = "/SAVE/BOOT.BIN";
uint8_t PAGE_BUFFER[SPM_PAGESIZE];

void load(const char* file_path);
void setup_button(void);
void setup_led(void);
void error_light(void);
void error_blink(void);
void reboot(void);

#define WATCHDOG_16MS   (_BV(WDE))

#endif
