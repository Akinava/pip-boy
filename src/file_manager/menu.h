#include <inttypes.h>

#ifndef MENU_H                                         
#define MENU_H

typedef struct {
  int8_t cursor;
  uint8_t lines;
  uint8_t max_lines;
} menu_t;
menu_t menu;

#endif
