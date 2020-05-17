#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <util/delay.h>


// defeine MACRO
// DDR Data Direction Register                                                  
#define SET_DDR_OUT(DDR, PIN) DDR |= _BV(PIN)                                   
#define SET_DDR_IN(DDR, PIN) DDR &= ~ _BV(PIN)                                  
#define SET_PULLUP(PORTS, PIN) PORTS |= _BV(PIN)                                
                                                                                 
// PORTS Data Register                                                          
#define SET_HIGH(PORTS, PIN) PORTS |= _BV(PIN)                                  
#define SET_LOW(PORTS, PIN) PORTS &= ~ _BV(PIN)                                 
#define TOGGLE(PORTS, PIN) PORTS ^= _BV(PIN);                                   
                                                                               
// PINS Input Pins Register                                                     
#define CHECK_PIN(PINS, PIN) !(PINS & _BV(PIN))                                 


// define PINS
// DEFINE BUTTON C                                                                 
#define BUTTON_C_DDR DDRC                                                         
#define BUTTON_C_PORT PORTC                                                       
#define BUTTON_C_PINS PINC                                                        
#define BUTTON_C_PIN PC3                                                     

// DEFINE LED
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD5

// DISPLAY
#define DISPLAY_DDR DDRC
#define DISPLAY_PORT PORTC
#define SDA PC4
#define SCL PC5

// SD
#define SD_DDR DDRB
#define SD_PORT PORTB
#define SD_CS PB2
#define MOSI PB3
#define MISO PB4
#define SCK PB5

// define BOOT
#define WATCHDOG_RESET  (_BV(WDIE) | _BV(WDP2) | _BV(WDP1))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))

const char BOOT_APP[] PROGMEM = "/BIN/FM.BIN";
const int (*app_start)(void) = 0x0;

void load_default_app(void);
void load_app_by_name(const char* file_path);
void load_app_by_cluster(uint16_t cluster, uint32_t size);

void setup_button_(void);
void setup_led_(void);
void error_light_(void);
void error_blink_(void);
void watchdog_config_(uint8_t x);

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
#define OBJECT_NAME_SIZE    8
#define OBJECT_EXT_SIZE     3

#define END_OF_CLASTERCHAIN 0x0fff8

#define CHAR_SLASH 0x2f
#define CHAR_DOT 0x2e
#define CHAR_SPACE 0x20

#define SD_SET SET_LOW
#define SD_UNSET SET_HIGH

#define SECTOR_BUFFER_SIZE 512

#define ROOT_CLUSTER 0

typedef struct {
  uint32_t sector;
  uint32_t size;
  uint16_t cluster;
} file_t;

file_t file;

uint8_t sector_buffer[SECTOR_BUFFER_SIZE];

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
uint8_t read_file_sector(file_t* file);
uint8_t find_file_by_path(const char* file_path);

uint8_t next_cluster_(void);
uint8_t copy_file_name(uint8_t* cursor, const char* file_path);
uint8_t next_claster_(void);
uint8_t compare_name(uint8_t* buf);
uint8_t next_sector(void);
uint8_t card_init_(void);
uint8_t vol_init_(void);
uint8_t find_obj_by_name(void);
uint32_t get_sector_by_cluster_(uint16_t cluster);
uint8_t check_obj_has_name_(void);
void cp_record_data_(uint8_t* buffer);
void erase_obj_name_(void);
void file_info_parce_(file_t* file, uint8_t* file_info);
void spi_send_(uint8_t data);
void card_command_(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t wait_start_block_(void);
uint8_t read_sector_(uint32_t sector);

static uint32_t volume_sector_;
static uint32_t fat_sector_;
static uint32_t root_sector_;
static uint32_t data_sector_;

static uint8_t obj_name_[OBJECT_NAME_SIZE+OBJECT_EXT_SIZE];
