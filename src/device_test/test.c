#include "test.h"

int main(void){
  display_begin();
  setup_keys();
  setup_speaker();
  setup_vibro();
  setup_led();

  event = NOOP;
  menu.cursor = 0;
  menu.page = PAGE_MAIN;

  while(1){
    show_menu();
    read_key();
    react_event();
  }
}

void show_menu(void){
  display_clean();
  if(menu.page == PAGE_MAIN){show_main_menu();};
  if(menu.page == TEST_DISPLAY){show_display_menu();};
  if(menu.page == TEST_SD){show_sd_menu();};
  if(menu.page == TEST_KEYS){show_keys_menu();};
  if(menu.page == TEST_SOUND){show_sound_menu();};
  if(menu.page == TEST_MIC){show_mic_menu();};
  if(menu.page == TEST_RADIO){show_radio_menu();};
  if(menu.page == TEST_BAT){show_bat_menu();};
  if(menu.page == TEST_VIBRO){show_vibro_led_menu();};
}

void react_event(void){
  switch(menu.page){
    case PAGE_MAIN:
      react_main_menu();
      break;
    case TEST_DISPLAY:
      react_display_menu();
      break;
    case TEST_SD:
      react_sd_menu();
      break;
    case TEST_KEYS:
      react_keys_menu();
      break;
    case TEST_SOUND:
      react_sound_menu();
      break;
    case TEST_MIC:
      react_mic_menu();
      break;
    case TEST_BAT:
      react_bat_menu();
      break;
    case TEST_VIBRO:
      react_vibro_led_menu();
      break;
    case TEST_RADIO:
      react_radio_menu();
      break;
  }
  event = NOOP;
}

void read_key(void){
  while(!event){
    if(CHECK_PIN(BUTTON_A_PINS, BUTTON_A_PIN)){event = A_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_B_PINS, BUTTON_B_PIN)){event = B_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){event = C_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_UP_PINS, BUTTON_UP_PIN)){event = UP_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_DOWN_PINS, BUTTON_DOWN_PIN)){event = DOWN_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_RIGHT_PINS, BUTTON_RIGHT_PIN)){event = RIGHT_KEY_PRESSED;}
    if(CHECK_PIN(BUTTON_LEFT_PINS, BUTTON_LEFT_PIN)){event = LEFT_KEY_PRESSED;}
  }
  // for the contact bounce
  _delay_ms(150);
}

void setup_vibro(void){
  SET_DDR_OUT(VIBRO_DDR, VIBRO_PIN);
}

void setup_led(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
}

void setup_speaker(void){
  SET_DDR_OUT(SPEAKER_DDR, SPEAKER_PIN);
}

void setup_keys(void){
  SET_DDR_IN(BUTTON_A_DDR, BUTTON_A_PIN);
  SET_PULLUP(BUTTON_A_PORT, BUTTON_A_PIN);
  SET_DDR_IN(BUTTON_B_DDR, BUTTON_B_PIN);
  SET_PULLUP(BUTTON_B_PORT, BUTTON_B_PIN);
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
  SET_DDR_IN(BUTTON_UP_DDR, BUTTON_UP_PIN);
  SET_PULLUP(BUTTON_UP_PORT, BUTTON_UP_PIN);
  SET_DDR_IN(BUTTON_DOWN_DDR, BUTTON_DOWN_PIN);
  SET_PULLUP(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
  SET_DDR_IN(BUTTON_LEFT_DDR, BUTTON_LEFT_PIN);
  SET_PULLUP(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
  SET_DDR_IN(BUTTON_RIGHT_DDR, BUTTON_RIGHT_PIN);
  SET_PULLUP(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN);
}

void show_main_menu(void){
  for (uint8_t i=0; i<MAX_LINES; i++){
    if (i==menu.cursor){
      print_invert(TEST_LIST[i], 0, i);
    }else{
      print(TEST_LIST[i], 0, i);
    }
  }
}

void react_main_menu(void){
  if (event == UP_KEY_PRESSED){menu.cursor--;}
  if (event == DOWN_KEY_PRESSED){menu.cursor++;}
  if (menu.cursor == -1){menu.cursor = 0;}
  if (menu.cursor == MAX_LINES){menu.cursor = MAX_LINES - 1;}
  if (event == A_KEY_PRESSED){menu.page = menu.cursor;}
  if (event == C_KEY_PRESSED){app_exit();}
}

void show_display_menu(void){
  print("display test", 0, 0);
  _delay_ms(2000);
  dispaly_test();
}

void react_display_menu(void){
  menu.page = PAGE_MAIN;
}

void show_sd_menu(void){
  print("test SD", 0, 0);
  print("SD card should be", 0, 2);
  print("in the reader", 0, 3);
  if (card_init()){
    print("test passed", 0, 5);
  }else{
    print("test failed", 0, 5);
  }
}

void react_sd_menu(void){
  menu.page = PAGE_MAIN;
}

void show_keys_menu(void){
  print("keys test", 0, 0);
  print("press the key", 0, 1);
}

void react_keys_menu(void){
  switch(event){
    case A_KEY_PRESSED:
      print("key A", 0, 3);
      break;
    case B_KEY_PRESSED:
      print("key B", 0, 3);
      break;
    case C_KEY_PRESSED:
      print("key C", 0, 3);
      print("exit", 0, 5);
      menu.page = PAGE_MAIN;
      break;
    case UP_KEY_PRESSED:
      print("key UP", 0, 3);
      break;
    case DOWN_KEY_PRESSED:
      print("key DOWN", 0, 3);
      break;
    case LEFT_KEY_PRESSED:
      print("key LEFT", 0, 3);
      break;
    case RIGHT_KEY_PRESSED:
      print("key RIGHT", 0, 3);
      break;
  }
  _delay_ms(500);
}

void show_sound_menu(void){
  print("sount test", 0, 0);
  print("press A", 0, 1);
}

void react_sound_menu(void){
  if (event == A_KEY_PRESSED){play_sound();}
  if (event == C_KEY_PRESSED){menu.page = PAGE_MAIN;}
}

void show_mic_menu(void){
  print("microphone test", 0, 0);
  test_mic();
}

void react_mic_menu(void){
  menu.page = PAGE_MAIN;
}

void show_radio_menu(void){
  print("radio test", 0, 0);

  if (radio_test()){
    print("test passed", 0, 2);
  }else{
    print("test failed", 0, 2);
  }
}

void react_radio_menu(void){
  menu.page = PAGE_MAIN;
}

void show_bat_menu(void){
  print("battery lvl test", 0, 0);
  test_bat_lvl();
}

void react_bat_menu(void){
  menu.page = PAGE_MAIN;
}

void show_vibro_led_menu(void){
  print("for test press:", 0, 0);
  print("A - led", 0, 2);
  print("B - vibro", 0, 3);
}
void react_vibro_led_menu(void){
  if (event == C_KEY_PRESSED){menu.page = PAGE_MAIN;}
  if (event == A_KEY_PRESSED){
    SET_HIGH(LED_PORT, LED_PIN);
    _delay_ms(500);
    SET_LOW(LED_PORT, LED_PIN);
  }
  if (event == B_KEY_PRESSED){
    SET_HIGH(VIBRO_PORT, VIBRO_PIN);
    _delay_ms(500);
    SET_LOW(VIBRO_PORT, VIBRO_PIN);
  }
}

void play_sound(void){
  for (uint16_t i=0; i<0xffff; i++){
    if (i%150){TOGGLE(SPEAKER_PORT, SPEAKER_PIN);}
  }
  SET_LOW(SPEAKER_PORT, SPEAKER_PIN);
}

void test_mic(void){
  init_mic();
  while(!CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    clean_buf();
    read_mic();
    _delay_ms(30);
  }
}

void init_mic(void){
  // ADMUX
  // set V AVCC
  UNSET(ADMUX, REFS1);
  SET(ADMUX, REFS0);

  // set V AREF
  //UNSET(ADMUX, REFS1);
  //UNSET(ADMUX, REFS0);

  // read bits direction
  UNSET(ADMUX, ADLAR);
  // set read ADC1
  UNSET(ADMUX, MUX3);
  UNSET(ADMUX, MUX2);
  UNSET(ADMUX, MUX1);
  SET(ADMUX, MUX0);

  // ADCSRA
  // set ADC on
  SET(ADCSRA, ADEN);
  // set ADPS 128
  SET(ADCSRA, ADPS2);
  SET(ADCSRA, ADPS1);
  SET(ADCSRA, ADPS0);
}

void read_mic(void){
  uint16_t vol1024;
  uint8_t vol127;
  // run DAC
  SET(ADCSRA, ADSC);
  // wait
  while(!(ADCSRA & (1 << ADIF)));
  // read
  vol1024 = (ADCL | ADCH << 8);

  // cast 1024 to 127
  vol127 = vol1024*127/1024;
  // show the line
  for (uint8_t i=0;i<vol127; i++){
    display_buff_[i] = 0xff;
  }
  display_update_(2);
}

void test_bat_lvl(void){
  init_bat_lvl();
  while(!CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    clean_buf();
    read_bat_lvl();
    _delay_ms(50);
  }
}

void init_bat_lvl(void){
  // ADMUX
  // set V AVCC
  UNSET(ADMUX, REFS1);
  SET(ADMUX, REFS0);

  // set V AREF
  //UNSET(ADMUX, REFS1);
  //UNSET(ADMUX, REFS0);

  // read bits direction
  UNSET(ADMUX, ADLAR);
  // set read ADC0
  UNSET(ADMUX, MUX3);
  UNSET(ADMUX, MUX2);
  UNSET(ADMUX, MUX1);
  UNSET(ADMUX, MUX0);

  // ADCSRA
  // set ADC on
  SET(ADCSRA, ADEN);
  // set ADPS 128
  SET(ADCSRA, ADPS2);
  SET(ADCSRA, ADPS1);
  SET(ADCSRA, ADPS0);
}

void read_bat_lvl(void){
  // run DAC
  SET(ADCSRA, ADSC);
  // wait
  while(!(ADCSRA & (1 << ADIF)));
  // read
  print8(ADCH, 0, 2);
  print8(ADCL, 20, 2);
}

void app_exit(void){
  display_clean();
  print("DON'T TURN OFF!", 0, 0);
  load_default_app();
}
