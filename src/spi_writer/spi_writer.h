#include <util/delay.h>
#include "macro.h"
#include "pins.h"
#include "display.h"
#include "menu.h"
#include "sd.h"

#ifndef SPI_WRITER_H
#define SPI_WRITER_H

#define load_default_app_addr 0x7ed2
#define load_default_app (*((void(*)(void))(load_default_app_addr/2)))

#define PROGRAM_ENABLE         0xac 
#define PROGRAM_ACKNOWLEDGE    0x53 
#define CHIP_ERASE             0x80
#define POOL_READY             0xF0
#define READ_SIGNATURE         0x30
#define WRITE_LOW_FUSE         0xa0
#define WRITE_HIGH_FUSE        0xa8
#define WRITE_EXT_FUSE         0xa4
#define LOAD_PROGRAM_LOW_BYTE  0x40
#define LOAD_PROGRAM_HIGH_BYTE 0x48
#define WRITE_PAGE             0x4c

#define NO_INSTRUCTION         0xff

#define ATMEGA328P 0
#define ATTINY85   1

#define HFUSE_PLACE 2
#define LFUSE_PLACE 6
#define EFUSE_PLACE 10

const uint8_t fuses_palce[] = {
  HFUSE_PLACE,
  LFUSE_PLACE,
  EFUSE_PLACE
};

const uint8_t write_fuses_command[] = {
  WRITE_HIGH_FUSE, 
  WRITE_LOW_FUSE, 
  WRITE_EXT_FUSE
};

typedef struct {
   uint8_t sig[3];    // chip signature
   uint8_t page_size;  // flash programming page size (bytes)
   uint8_t fuses[3];  // fuses high/low/ext / default
   uint16_t ram_size; // chip ram size / bytes
} signatureType;

signatureType signatures [] = {
  {{0x1e, 0x95, 0x0f}, 128, {0xda, 0xff, 0xfd}, 0x8000},
  {{0x1e, 0x93, 0x0b}, 64, {0xdf, 0x62, 0xff}, 0x2000}
};

void show_menu(void);
void react_event(void);
void spi_activate(void);
void spi_deactivate(void);
void spi_send(uint8_t data);
uint8_t program_enable(void);
uint8_t read_signature(uint8_t chip);
void erise_chip(void);
void write_fuse(uint8_t fuse, uint8_t value);
void load_program(void);
void busy_wait(void);
void isp_command(uint8_t cmd0, uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);
void app_exit(void);
void show_main_menu(void);
void react_main_menu(void);
void show_load_fuses_menu(void);
void react_load_fuses(void);
void react_write_fuses(void);
void react_fuses_edit(void);
void show_fuses_edit(void);
void react_fuses_edit(void);
void up_fuse(void);
void down_fuse(void);
void show_load_app_menu(void);
void react_load_app(void);
void show_app_addr_set_menu(void);
void react_app_addr_set(void);
void up_load_addr(void);
void down_load_addr(void);
void react_app_write(void);
void react_choose_the_file(void);
uint8_t program_firmware(void);
uint8_t init_chip(uint8_t chip);
void author(void);

// PAGES MAGIC NUMBER
#define PAGE_LOAD_APP   0
#define PAGE_LOAD_FUSES 1
#define MAX_LINES       2
#define PAGE_MAIN       MAX_LINES

#define PAGE_FUSES_EDIT  3
#define PAGE_FUSES_WRITE 4

#define PAGE_APP_ADDR_SET 5
#define PAGE_APP_FILE     6
#define PAGE_APP_WRITE    7

char* MENU_LIST[] = {
  "load app    ",
  "change fuses",
};

int8_t sub_coursor;

#define APP_ADDR_INDENT 10 
#define CHAR_DOT        0x2e
#define CHAR_SPACE      0x20

uint16_t app_addr_start = 0x7800;
char app_name_buf[] = "file:      no file";
uint16_t app_file_cluster = 0;
uint32_t app_file_size = 0;

#endif
