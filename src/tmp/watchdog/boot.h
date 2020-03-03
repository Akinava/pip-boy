#include <avr/boot.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "display.h"
#include "macro.h"
#include "pins.h"

#ifndef BOOT_H                                         
#define BOOT_H

void setup_button_(void);
void setup_led_(void);

#endif
