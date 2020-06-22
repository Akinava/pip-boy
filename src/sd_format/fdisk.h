#include "display.h"
#include "sd.h"

#ifndef FDISK_H
#define FDISK_H

typedef struct {
  char name[5];
  uint32_t sise;
} size_ref_t;

size_ref_t size_ref[] = {
  {"8Gb ", 0x00e6d000},
  {"16Gb", 0x01cd8000},
  {"32Gb", 0x03b85000},
};

uint8_t cursor = 0;

void author(void);

#endif
