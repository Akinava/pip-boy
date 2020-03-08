#include <avr/pgmspace.h>
#include "macro.h"
#include "pins.h"

#ifndef SD_H
#define SD_H

// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
#define CMD0     0      
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
#define CMD8     8
/** APP_CMD - escape for application specific command */
#define CMD55    55 
/** READ_BLOCK - read a single data block from the card */
#define CMD17    17
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
#define ACMD41   41  
/** READ_OCR - read the OCR register of a card */
#define CMD58    58

/** status for card in the ready state */
#define R1_READY_STATE 0
/** status for card in the idle state */
#define R1_IDLE_STATE 1

/** start data token for read or write */
#define DATA_START_BLOCK 0xfe

#define VOL_ADDRESS_OFFSET 0x01c6

#define VOL_INFO_OFFSET 0x0b

#define FILE_CLUSTER_OFFSET 0x1a
#define FILE_SIZE_OFFSET 0x1c

#define OBJECT_RECORD_SIZE 32
#define OBJECT_NAME_SIZE 8 + 3

#define END_OF_CLASTERCHAIN 0x0fff8

#define CHAR_SLASH 0x2f
#define CHAR_DOT 0x2e
#define CHAR_SPACE 0x20

#define SD_SET SET_LOW
#define SD_UNSET SET_HIGH

typedef struct {
  uint32_t sector; // start of file
  uint32_t size;
  uint16_t cluster;
  uint32_t cursor;
} file_t;

uint8_t sector_buffer[512];

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

vol_info_t vol_info;

uint8_t sd_init(void);
uint8_t file_open(const char* file_path, file_t* boot_file);
uint8_t file_read_sector(file_t* file);

uint8_t card_init_(void);
uint8_t vol_init_(void);
uint8_t find_obj_by_name(file_t* file);
uint8_t next_claster_(file_t* file);
void get_sector_by_cluster_(file_t* file);
uint8_t check_obj_has_name_(void);
void cp_record_data_(uint8_t* buffer);
void erase_obj_name_(void);
void file_info_parce_(file_t* file, uint8_t* file_info);
void spi_send_(uint8_t data);
void card_command_(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t wait_start_block_(void);
uint8_t read_sector_(uint32_t sector);

uint32_t volume_sector_;
uint32_t fat_sector_;
uint32_t root_sector_;
uint32_t data_sector_;

uint8_t obj_name_[OBJECT_NAME_SIZE];
uint8_t obj_data_[OBJECT_RECORD_SIZE];

#endif
