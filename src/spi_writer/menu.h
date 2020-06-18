#include <util/delay.h>
#include "pins.h"
#include "macro.h"

#ifndef MENU_H
#define MENU_H

// KEYS MAGIC NUMBERS
#define NOOP 0
#define A_KEY_PRESSED 1
#define B_KEY_PRESSED 2
#define C_KEY_PRESSED 3
#define UP_KEY_PRESSED 4
#define DOWN_KEY_PRESSED 5
#define LEFT_KEY_PRESSED 6
#define RIGHT_KEY_PRESSED 7

typedef struct {
  int8_t cursor;
  uint8_t page;
  uint8_t event;
} menu_t;

menu_t menu;

void menu_init(void);
void setup_keys(void);
void read_key(void);

#endif
