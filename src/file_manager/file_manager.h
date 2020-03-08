#include <avr/io.h>
#include <string.h>
#include "display.h"
#include "sd.h"
#include "macro.h"
#include "pins.h"

#ifndef FILE_MANAGER_H                                         
#define FILE_MANAGER_H

void make_list(void);
void show_list(void);

uint8_t full_path_buf[64];
uint8_t list_lines;
uint8_t cursor;
uint8_t top_line;

/*
 * one recodr in dir is 32 bytes
 * one sector 512 bytes
 * 512/32=16 records per sector
 * 8 bytes name 3 bytes extension
 * +1 byte zero
 */
char list_buf[16][8+3+1];

#endif
