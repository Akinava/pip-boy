#include <util/delay.h>
#include "macro.h"
#include "pins.h"

#ifndef RADIO_H
#define RADIO_H

#define RADIO_NOOP 0xFF
#define RADIO_DEFAULT_STATE 0x0E

uint8_t radio_test(void);
void spi_send(uint8_t data);

#endif
