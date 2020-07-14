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

uint16_t MBR_BOOT_LOADER_CODE_SIZE = 0x1be;
uint8_t MBR_VOLUME_RECORD_SIZE = 0x10;

uint16_t END_OF_BLOCK_OFFSET = 0x1fe;
uint8_t END_OF_BLOCK[] = {0x55, 0xaa};


uint8_t MBR_VOLUME_TYPE_OFFSET = 0x04;
uint8_t MBR_VOLUME_TYPE_FAT16[] = {0x06};
uint8_t MBR_VOLUME_TYPE_FAT32[] = {0x0b};

uint8_t MBR_START_SECTOR_OFFSET = 0x08;
uint8_t MBR_PART1_START_SECTOR[] = {0x00, 0x00, 0x00, 0x01};
uint8_t MBR_PART2_START_SECTOR[] = {0x00, 0x3f, 0xff, 0x42};

uint8_t MBR_PART_SIZE_OFFSET = 0x0c;
uint8_t MBR_PART1_SIZE[] = {0x00, 0x3f, 0xff, 0x41};



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
