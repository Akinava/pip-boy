#include <inttypes.h>

#ifndef MENU_H                                         
#define MENU_H

#define ABOVE 0
#define BELOW 1

typedef struct {
  int8_t cursor;
  uint8_t lines;
  uint8_t max_lines;
  uint8_t next_page;
} menu_t;
menu_t menu;

#endif
