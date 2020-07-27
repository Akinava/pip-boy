#include <util/delay.h>
#include "spi.h"

#ifndef RADIO_H
#define RADIO_H

#define RADIO_NOOP 0xFF
#define RADIO_DEFAULT_STATE 0x0E

uint8_t radio_init(void);

#endif
