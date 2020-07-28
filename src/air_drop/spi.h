#include "macro.h"
#include "pins.h"

#ifndef SPI_H
#define SPI_H

#define SPI_UNSET SET_HIGH
#define SPI_SET SET_LOW

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

void spi_init(void);
void spi_set_speed(uint8_t clock);

void spi_send(uint8_t data);
uint8_t spi_receive(void);

#endif
