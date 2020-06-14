#include <util/delay.h>
#include "macro.h"
#include "pins.h"
#include "display.h"

#ifndef SPI_WRITER_H
#define SPI_WRITER_H

#define PROGRAM_ENABLE         0xac 
#define PROGRAM_ACKNOWLEDGE    0x53 
#define CHIP_ERASE             0x80
#define READ_SIGNATURE         0x30
#define WRITE_LOW_FUSE         0xa0
#define WRITE_HIGH_FUSE        0xa8
#define WRITE_EXT_FUSE         0xa4
#define LOAD_PROGRAM_LOW_BYTE  0x40
#define LOAD_PROGRAM_HIGH_BYTE 0x48
#define WRITE_PAGE             0x4c

void spi_send(uint8_t data);
void program_enable(void);
void read_signature(void);
void erise_chip(void);
void write_fuse(uint8_t fuse, uint8_t value);
void load_program(void);
void isp_command(uint8_t cmd0, uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);

#endif
