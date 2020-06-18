#include <inttypes.h>

#ifndef SD_MENU_H                                         
#define SD_MENU_H

#define ABOVE 0
#define BELOW 1

typedef struct {
  int8_t cursor;
  uint8_t lines;
  uint8_t max_lines;
  uint8_t next_page;
} sd_menu_t;

sd_menu_t sd_menu;

#endif
