#include <avr/pgmspace.h>
#include "macro.h"
#include "pins.h"

#ifndef SD_H
#define SD_H

// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
#define CMD0     0x00      
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
#define CMD8     0x08
/** APP_CMD - escape for application specific command */
#define CMD55    0x37 
/** READ_BLOCK - read a single data block from the card */
#define CMD17    0x11
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
#define ACMD41   0x29   
/** READ_OCR - read the OCR register of a card */
#define CMD58    0x3a

/** status for card in the ready state */
#define R1_READY_STATE 0
/** status for card in the idle state */
#define R1_IDLE_STATE 0x01
/** Standard capacity V2 SD card */
#define SD_CARD_TYPE_SD2 2
/** High Capacity SD card */
#define SD_CARD_TYPE_SDHC 3

/** start data token for read or write */
#define DATA_START_BLOCK 0Xfe

#define VOL_ADDRESS_OFFSET 0x01c6

#define SECTOR_LENGTH 4

#define VOL_INFO_OFFSET 0x0b
//#define VOL_INFO_SIZE 13

#define OBJECT_RECORD_SIZE 32
#define OBJECT_RECORD_NAME_OFFSET 0
#define OBJECT_NAME_SIZE 8 + 3

#define END_OF_CLASTERCHAIN 0x0fff8

#define CHAR_SLASH 0x2f
#define CHAR_DOT 0x2e
#define CHAR_SPACE 0x20

typedef struct {
  uint32_t sector;
  uint32_t size;
  uint16_t cluster;
  uint32_t cursor;
} file_t;

uint8_t sd_init(void);
uint8_t file_open(const char* file_path, file_t* boot_file);
uint8_t file_read(file_t* file, uint8_t* buf, uint16_t size);

static uint8_t card_init_(void);
static uint8_t vol_init_(void);
static uint8_t find_obj_by_name(uint8_t* obj_name, file_t* file);
static uint8_t next_claster_(file_t* file);
void get_sector_by_cluster_(file_t* file);
static uint8_t cmp_(uint8_t* s1, uint8_t* s2);
static void memset_(uint8_t* s1, uint8_t c, uint8_t size);
uint32_t warp_bytes_(uint8_t* file_info, const uint8_t* rule);
static void file_info_parce_(file_t* file, uint8_t* file_info);
static void spi_send_(uint8_t data);
static uint8_t spi_rec_(void);
uint8_t cd_raw_read_(uint32_t block, uint16_t offset, uint8_t *dst, uint16_t count);
static uint8_t card_command_(uint8_t cmd, uint32_t arg, uint8_t crc);
void read_end_(void);
static uint8_t sd_wait_start_block_(void);

typedef struct {
  uint16_t bytes_per_sector;
  uint8_t sectors_per_claster;
  uint16_t reserved_sectors;
  uint8_t number_of_FATs;
  uint16_t root_directory_entries;
  uint16_t total_logical_sectors;
  uint8_t media_descriptor;
  uint16_t sectors_per_FAT; 
} vol_info_t;

vol_info_t vol_info_;

static uint32_t fat_sector_;
static uint32_t root_sector_;
static uint32_t data_sector_;
static uint8_t obj_name_[OBJECT_NAME_SIZE];

static uint16_t offset_;
static uint8_t in_block_;
static uint8_t type_;
static uint32_t block_;
static uint8_t partial_block_read_;
static uint32_t volume_sector_;

#endif
