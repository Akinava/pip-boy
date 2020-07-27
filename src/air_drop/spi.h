#include "macro.h"
#include "pins.h"

#ifndef SPI_H
#define SPI_H

#define SPI_UNSET SET_HIGH
#define SPI_SET SET_LOW

void spi_init(void);
void spi_send(uint8_t data);

#endif
