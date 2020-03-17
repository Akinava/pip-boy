#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "display.h"
#include "sd.h"
#include "macro.h"
#include "pins.h"

#ifndef FILE_MANAGER_H                                         
#define FILE_MANAGER_H

uint8_t make_list(void);
void show_list(void);
void read_keyboard(void);
void keys_setup(void);

uint8_t full_path_buf[64];
uint8_t lines;
uint8_t cursor;

/*
 * 8 line in display
 * 8 bytes name 3 bytes extension
 * +1 byte zero
 */
uint16_t parents_cluster;

#define LINES 8
obj_data_t objects_data[LINES];

void copy_line_(char* buf, uint8_t y);
void clean_obj_data_(void);

#endif
