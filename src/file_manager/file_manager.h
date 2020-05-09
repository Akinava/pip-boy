#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "display.h"
#include "sd.h"
#include "macro.h"
#include "pins.h"
#include "menu.h"

#ifndef FILE_MANAGER_H                                         
#define FILE_MANAGER_H

uint8_t make_list(void);
void show_page(void);
void read_keyboard(void);
void keys_setup(void);
void jump_cursor(void);
void step_cursor_back(void);
uint8_t cursor_in_page(void);
// max forders nesting is 6
uint8_t full_path_buf[64];

/*
 * 8 line in display
 * 8 bytes name 3 bytes extension
 * +1 byte zero
 */

#define LINES 8
obj_data_t objects_data[LINES];

void copy_line_(char* buf, uint8_t y);

#endif
