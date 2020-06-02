#include <avr/io.h>
#include <util/delay.h>
#include "display.h"
#include "sd.h"
#include "macro.h"
#include "pins.h"

#ifndef TEST_H                                         
#define TEST_H

#define load_app_by_cluster_addr 0x7b9a
#define load_app_by_cluster (*((void(*)(uint16_t cluster, uint32_t size))(load_app_by_cluster_addr/2)))

// PAGES MAGIC NUMBER
#define TEST_DISPLAY 0
#define TEST_SD 1
#define TEST_KEYS 2
#define TEST_SOUND 3
#define TEST_MIC 4
#define TEST_RADIO 5
#define TEST_BAT 6
#define TEST_VIBRO 7
#define MAX_LINES 8
#define PAGE_MAIN MAX_LINES

// KEYS MAGIC NUMBERS
#define NOOP 0
#define A_KEY_PRESSED 1
#define B_KEY_PRESSED 2
#define C_KEY_PRESSED 3
#define UP_KEY_PRESSED 4
#define DOWN_KEY_PRESSED 5
#define LEFT_KEY_PRESSED 6
#define RIGHT_KEY_PRESSED 7

char* TEST_LIST[] = {
  "dispaly    ",
  "sd reader  ",
  "keys       ",
  "sound      ",
  "microphon  ",
  "radio link ",
  "battery lvl",
  "vibro & led"
};

typedef struct {
  int8_t cursor;
  uint8_t lines;
  uint8_t page;
} menu_t;
menu_t menu;

uint8_t event;

void setup_vibro(void);
void setup_led(void);
void setup_speaker(void);
void setup_keys(void);
void show_menu(void);
void read_key(void);
void react_event(void);

void show_main_menu(void);
void react_main_menu(void);
void show_display_menu(void);
void react_display_menu(void);
void show_sd_menu(void);
void react_sd_menu(void);
void show_keys_menu(void);
void react_keys_menu(void);
void show_sound_menu(void);
void react_sound_menu(void);
void show_mic_menu(void);
void react_mic_menu(void);
void show_radio_menu(void);
void react_radio_menu(void);
void show_bat_menu(void);
void react_bat_menu(void);
void show_vibro_led_menu(void);
void react_vibro_led_menu(void);
void play_sound(void);
void init_mic(void);
void read_mic(void);

#endif
