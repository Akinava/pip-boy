#ifndef BOOTLOADER_H                                                                  
#define BOOTLOADER_H                                                                  


#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


#include "macros.h"
#include "pins.h"


const char default_program[] PROGMEM = "boot";
void load(const char * filename);

#endif
