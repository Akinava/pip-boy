#include "../macro.h"
#include "../pins.h"
#include "../display_oled_i2c/display.h"


#ifndef SD_H
#define SD_H

uint8_t sdBegin(void);
uint8_t fileOpen(void);
uint8_t fileAvailable(void);
uint8_t fileRead(void);
void fileClose(void);
void sdStop(void);

uint8_t card_init(void);
uint8_t vol_init(void);
void SdFileSystem_begin(void);
void spiSend(uint8_t data);
uint8_t spiRec(void);
uint8_t cardCommand(uint8_t cmd, uint32_t arg, uint8_t crc);
void readEnd(void);
uint8_t cdRawRead(uint32_t block, uint16_t offset, uint8_t *dst, uint16_t count);
uint8_t sdWaitStartBlock(void);
void error(uint8_t e);

uint16_t offset_;
uint8_t inBlock_;
uint8_t type_;
uint32_t block_;
uint8_t partialBlockRead_;
uint8_t fatCount_;
uint8_t blocksPerCluster_;
uint32_t blocksPerFat_;
int16_t rootDirEntryCount_;
uint32_t fatStartBlock_;
uint32_t rootDirStart_;
uint32_t dataStartBlock_;
uint32_t totalBlocks_;
uint32_t clusterCount_;
uint8_t fatType_;

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

/** offset to partition table in mbr */
#define PART_OFFSET (512-64-2) 
/** Byte count for part of BIOS Parameter Block to be read by init() */
#define BPB_COUNT 37
/** start data token for read or write */
#define DATA_START_BLOCK      0XFE

struct biosPramBlock{
          /**
           * Count of bytes per sector. This value may take on only the
           * following values: 512, 1024, 2048 or 4096
           */
  uint16_t bytesPerSector;
          /**
           * Number of sectors per allocation unit. This value must be a
           * power of 2 that is greater than 0. The legal values are
           * 1, 2, 4, 8, 16, 32, 64, and 128.
           */
  uint8_t  sectorsPerCluster;
          /**
           * Number of sectors before the first FAT.
           * This value must not be zero.
           */
  uint16_t reservedSectorCount;
          /** The count of FAT data structures on the volume. This field should
           *  always contain the value 2 for any FAT volume of any type.
           */
  uint8_t  fatCount;
          /**
          * For FAT12 and FAT16 volumes, this field contains the count of
          * 32-byte directory entries in the root directory. For FAT32 volumes,
          * this field must be set to 0. For FAT12 and FAT16 volumes, this
          * value should always specify a count that when multiplied by 32
          * results in a multiple of bytesPerSector.  FAT16 volumes should
          * use the value 512.
          */
  uint16_t rootDirEntryCount;
          /**
           * This field is the old 16-bit total count of sectors on the volume.
           * This count includes the count of all sectors in all four regions
           * of the volume. This field can be 0; if it is 0, then totalSectors32
           * must be non-zero.  For FAT32 volumes, this field must be 0. For
           * FAT12 and FAT16 volumes, this field contains the sector count, and
           * totalSectors32 is 0 if the total sector count fits
           * (is less than 0x10000).
           */
  uint16_t totalSectors16;
          /**
           * This dates back to the old MS-DOS 1.x media determination and is
           * no longer usually used for anything.  0xF8 is the standard value
           * for fixed (non-removable) media. For removable media, 0xF0 is
           * frequently used. Legal values are 0xF0 or 0xF8-0xFF.
           */
  uint8_t  mediaType;
          /**
           * Count of sectors occupied by one FAT on FAT12/FAT16 volumes.
           * On FAT32 volumes this field must be 0, and sectorsPerFat32
           * contains the FAT size count.
           */
  uint16_t sectorsPerFat16;
           /** Sectors per track for interrupt 0x13. Not used otherwise. */
  uint16_t sectorsPerTrtack;
           /** Number of heads for interrupt 0x13.  Not used otherwise. */
  uint16_t headCount;
          /**
           * Count of hidden sectors preceding the partition that contains this
           * FAT volume. This field is generally only relevant for media
           *  visible on interrupt 0x13.
           */
  uint32_t hidddenSectors;
          /**
           * This field is the new 32-bit total count of sectors on the volume.
           * This count includes the count of all sectors in all four regions
           * of the volume.  This field can be 0; if it is 0, then
           * totalSectors16 must be non-zero.
           */
  uint32_t totalSectors32;
          /**
           * Count of sectors occupied by one FAT on FAT32 volumes.
           */
  uint32_t sectorsPerFat32;
          /**
           * This field is only defined for FAT32 media and does not exist on
           * FAT12 and FAT16 media.
           * Bits 0-3 -- Zero-based number of active FAT.
           *             Only valid if mirroring is disabled.
           * Bits 4-6 -- Reserved.
           * Bit 7	-- 0 means the FAT is mirrored at runtime into all FATs.
	         *        -- 1 means only one FAT is active; it is the one referenced in bits 0-3.
           * Bits 8-15 	-- Reserved.
           */
  uint16_t fat32Flags;
          /**
           * FAT32 version. High byte is major revision number.
           * Low byte is minor revision number. Only 0.0 define.
           */
  uint16_t fat32Version;
          /**
           * Cluster number of the first cluster of the root directory for FAT32.
           * This usually 2 but not required to be 2.
           */
  uint32_t fat32RootCluster;
          /**
           * Sector number of FSINFO structure in the reserved area of the
           * FAT32 volume. Usually 1.
           */
  uint16_t fat32FSInfo;
          /**
           * If non-zero, indicates the sector number in the reserved area
           * of the volume of a copy of the boot record. Usually 6.
           * No value other than 6 is recommended.
           */
  uint16_t fat32BackBootBlock;
          /**
           * Reserved for future expansion. Code that formats FAT32 volumes
           * should always set all of the bytes of this field to 0.
           */
  uint8_t  fat32Reserved[12];
};

/** Type name for biosParmBlock */
typedef struct biosPramBlock bpb_t;

#endif
