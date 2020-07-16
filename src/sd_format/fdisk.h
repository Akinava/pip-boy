#include <util/delay.h>
#include "display.h"
#include "sd.h"

#ifndef FDISK_H
#define FDISK_H

#define load_default_app_addr 0x7ed2
#define load_default_app (*((void(*)(void))(load_default_app_addr/2)))


#define END_MBR 0x55aa

#define NOOP 0
#define A_KEY_PRESSED 1
#define C_KEY_PRESSED 2

#define PART1_START_SECTOR 0x00000001
#define PART2_START_SECTOR 0x003fff42

#define SECTORS_PER_FAT16_TABLE 0x0100
#define RESERVED_SECTORS_BEFORE_FAT16_TABLE 1
#define FAT16_TABLES_DEF 2

uint16_t END_OF_BLOCK_OFFSET = 0x1fe;
uint8_t END_OF_BLOCK[] = {0xaa, 0x55};

uint32_t VOLUME_ID;

typedef struct {
  char name[5];
  uint32_t sise;
} size_ref_t;

size_ref_t size_ref[] = {
  {"8Gb ", 0x00e6d000},
  {"16Gb", 0x01cd8000},
  {"32Gb", 0x03b85000},
};

int8_t cursor;
uint8_t event;
uint32_t card_size;

void format_menu(void);
void display_page(void);
void read_key(void);
void react(void);
void setup_keys(void);
uint8_t format(void);
void app_exit(void);
void author(void);
void mbr(void);
void fat16(void);
void fat32(void);
void copy_data(uint8_t* dst, uint8_t* src, uint16_t length);
void get_part2_size(uint8_t* mbr_part2_size);

#endif
