#include <util/delay.h>
#include "display.h"
#include "sd.h"

#ifndef FDISK_H
#define FDISK_H

#define NOOP 0
#define A_KEY_PRESSED 1
#define C_KEY_PRESSED 2

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
void format_menu(void);
void display_page(void);
void read_key(void);
void react(void);
void setup_keys(void);
uint8_t format(void);
void author(void);

#endif
