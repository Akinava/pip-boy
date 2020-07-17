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

uint16_t OEM_NAME_OFFSET = 0x03;
uint8_t OEM_NAME[] = {'t', 'a', 'f', '.', 's', 'f', 'k', 'm'};

uint16_t BYTE_PER_SECTOR_OFFSET = 0x0b;
uint8_t BYTE_PER_SECTOR[] = {0x02, 0x00};

uint16_t END_OF_BLOCK_OFFSET = 0x1fe;
uint8_t END_OF_BLOCK[] = {0xaa, 0x55};

uint16_t SECTORS_PER_CLUSTER_OFFSET = 0x0d;
uint16_t FAT_TABLES_OFFSET = 0x10;
uint8_t FAT_TABLES[] = {0x02};

uint16_t MEDIA_DESCRIPTOR_OFFSET = 0x15;
uint8_t MEDIA_DESCRIPTOR[] = {0xf8};

uint16_t PARTITION_SIZE_OFFSET = 0x20;
 
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
uint32_t sectors_per_fat32_volue;

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
void get_part2_size(uint8_t* part2_size);
void get_sectors_per_fat32(uint8_t* sectors_per_fat32);

#endif
