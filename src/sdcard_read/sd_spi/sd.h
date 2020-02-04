#include <string.h>
#include "../macro.h"
#include "../pins.h"
#include "../display_oled_i2c/display.h"

#ifndef SD_H
#define SD_H

uint8_t sd_init(void);

uint8_t card_init(void);
uint8_t vol_init(void);
uint8_t root_dir_init(void);

void spi_send(uint8_t data);
uint8_t spi_rec(void);

uint8_t cd_raw_read(uint32_t block, uint16_t offset, uint8_t *dst, uint16_t count);
uint8_t card_command(uint8_t cmd, uint32_t arg, uint8_t crc);
void read_end(void);
uint8_t sd_wait_start_block(void);

uint16_t offset_;
uint8_t in_block_;
uint8_t type_;
uint32_t block_;
uint8_t partial_block_read_;

uint32_t volume_address_;


//uint8_t fat_count_;
//uint8_t blocks_per_cluster_;
//uint32_t blocksPerFat_;
//int16_t rootDirEntryCount_;
//uint32_t fatStartBlock_;
//uint32_t rootDirStart_;
//uint32_t dataStartBlock_;

// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
#define CMD0     0X00      
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
#define CMD8     0X08
/** APP_CMD - escape for application specific command */
#define CMD55    0X37 
/** READ_BLOCK - read a single data block from the card */
#define CMD17    0X11
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
#define ACMD41   0X29   
/** READ_OCR - read the OCR register of a card */
#define CMD58    0X3A

/** status for card in the ready state */
#define R1_READY_STATE 0
/** status for card in the idle state */
#define R1_IDLE_STATE 0x01
/** Standard capacity V2 SD card */
#define SD_CARD_TYPE_SD2 2
/** High Capacity SD card */
#define SD_CARD_TYPE_SDHC 3

/** start data token for read or write */
#define DATA_START_BLOCK      0XFE

#define VOL_ADDRESS_OFFSET 0x01c6
#define VOL_ADDRESS_COUNT 4

#define ROOT_DIR_INFO_OFFSET 0x0b 
#define ROOT_DIR_INFO_COUNT 13

#endif
