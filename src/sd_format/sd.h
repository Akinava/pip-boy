//#include <avr/pgmspace.h>
#include <string.h>
#include "macro.h"
#include "pins.h"
#include "menu.h"

// FIXME
#include "display.h"

#ifndef SD_H
#define SD_H

// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
#define CMD0     0      
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
#define CMD8     8
/** SEND_CSD - read the Card Specific Data (CSD register) */
#define CMD9     9
/** SEND_CID - read the card identification information (CID register) */
#define CMD10    10
/** WRITE_BLOCK - write a single data block to the card */
#define CMD24    24
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
/** write data accepted token */
#define DATA_RES_ACCEPTED 0x05

#define SECTOR_SIZE               512

/** start data token for read or write */
#define DATA_START_BLOCK          0xfe

#define VOL_ADDRESS_OFFSET        0x01c6

#define VOL_INFO_OFFSET           0x0b

#define DATA_CLUSTER_OFFSET       0x1a
#define FAT_CLUSTER_SIZE          2  
#define FAT_CLUSTER_SKIP          2

#define OBJECT_RECORD_SIZE        32
#define OBJECT_NAME_SIZE          8+3

#define END_OF_CLUSTERCHAIN       0x0fff8

#define OBJECT_IS_DELETED         0xe5
#define OBJECT_NOT_EXIST          0x0

#define OBJ_ATTRIBUTES_OFFSET     0x0b
#define OBJ_CATALOG               0x10

#define OBJ_SIZE_OFFSET           0x1c

#define SD_SET SET_LOW
#define SD_UNSET SET_HIGH

#define FILE_FLAG 0
#define DIR_FLAG 1

#define ROOT_CLUSTER 0

typedef struct {
  uint8_t dir;
  uint16_t data_cluster;
  uint16_t cluster;  // 0 for root dir
  uint32_t sector;
  uint16_t sector_offset;
  char name[OBJECT_NAME_SIZE];
  uint32_t size;
} obj_data_t;

uint8_t sector_buffer[SECTOR_SIZE];

typedef struct {
  uint16_t bytes_per_sector;       // 0x0200    512
  uint8_t sectors_per_cluster;     // 0x40      64
  uint16_t reserved_sectors;       // 0x0040
  uint8_t number_of_FATs;          // 0x02
  uint16_t root_directory_entries; // 0x0400
  uint16_t total_logical_sectors;  
  uint8_t media_descriptor;
  uint16_t sectors_per_FAT;        // 0x0100

  uint32_t start_sector;           // 0x0800 
  uint32_t fat_table_sector;       // 0x0840
  uint32_t root_sector;            // 0x0a40 
  uint32_t data_sector;            // 0x0a80
  uint16_t primary_dir_cluster;
} vol_info_t;

vol_info_t vol_info;

uint8_t sd_init(void);
uint8_t read_directory_page(menu_t* menu, obj_data_t* objects_data);

uint8_t card_init_(void);
uint8_t vol_init_(void);
uint8_t get_card_size(uint32_t* card_size);
void cp_obj_name_(char* dst, uint16_t buffer_offset);
uint8_t copy_obj_to_page_(obj_data_t* objects_data, obj_data_t* src_obj, menu_t* menu);
uint8_t next_cluster_by_fat_(obj_data_t* prev_object, obj_data_t* next_object);
uint32_t get_sector_by_cluster_(obj_data_t* object);
void spi_send_(uint8_t data);
void card_command_(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t wait_start_block_(void);
uint8_t read_sector_(uint32_t sector);
uint8_t write_sector_(uint32_t sector);

#endif
