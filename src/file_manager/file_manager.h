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

#define load_app_by_cluster_addr 0x7880
#define load_app_by_cluster (*((void(*)(uint16_t cluster, uint32_t size))(load_app_by_cluster_addr/2)))

uint8_t make_list(void);
void show_page(void);
uint8_t read_keyboard(void);
void setup_keys(void);
void jump_cursor(void);
void step_cursor_back(void);
uint8_t check_cursor_in_page(void);

/*
 * 8 line in display
 * 8 bytes name 3 bytes extension
 * +1 byte zero
 */

#define LINES 8
obj_data_t objects_data[LINES];

void copy_line_(char* buf, uint8_t y);
uint8_t compose_obj_name(obj_data_t obj, char* buff_dst);

#endif
