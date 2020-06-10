#include "display.h"

const uint8_t OLED_INIT_COMMANDS[] PROGMEM = {
  0xAE, // SSD1306_DISPLAY_OFF
  0xD5, // SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO
  0x80,
  0xA8, // SSD1306_SET_MULTIPLEX_RATIO
  0x3F,
  0xD3, // SSD1306_SET_DISPLAY_OFFSET
  0x00,
  SSD1306_SET_START_LINE | 0x0,
  0x8D, // SSD1306_CHARGE_PUMP
  0x14,
  0x20, // SSD1306_MEMORY_ADDR_MODE
  0x00,
  SSD1306_SET_SEGMENT_REMAP | 0x1,
  0xC8, // SSD1306_COM_SCAN_DIR_DEC
  0xDA, // SSD1306_SET_COM_PINS
  0x12,
  0x81, // SSD1306_SET_CONTRAST_CONTROL
  0xCF,
  0xD9, // SSD1306_SET_PRECHARGE_PERIOD
  0xF1,
  0xDB, // SSD1306_SET_VCOM_DESELECT
  0x40,
  0xA4, // SSD1306_DISPLAY_ALL_ON_RESUME
  0xA6, // SSD1306_NORMAL_DISPLAY
  0xAF, // SSD1306_DISPLAY_ON
};

const uint8_t FONT[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00,   // sp
  0x00, 0x00, 0x2f, 0x00, 0x00,   // !
  0x00, 0x07, 0x00, 0x07, 0x00,   // "
  0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
  0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
  0x23, 0x13, 0x08, 0x64, 0x62,   // %
  0x36, 0x49, 0x55, 0x22, 0x50,   // &
  0x00, 0x05, 0x03, 0x00, 0x00,   // '
  0x00, 0x1c, 0x22, 0x41, 0x00,   // (
  0x00, 0x41, 0x22, 0x1c, 0x00,   // )
  0x14, 0x08, 0x3E, 0x08, 0x14,   // *
  0x08, 0x08, 0x3E, 0x08, 0x08,   // +
  0x00, 0x00, 0xA0, 0x60, 0x00,   // ,
  0x08, 0x08, 0x08, 0x08, 0x08,   // -
  0x00, 0x60, 0x60, 0x00, 0x00,   // .
  0x20, 0x10, 0x08, 0x04, 0x02,   // /

  0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
  0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
  0x42, 0x61, 0x51, 0x49, 0x46,   // 2
  0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
  0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
  0x27, 0x45, 0x45, 0x45, 0x39,   // 5
  0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
  0x01, 0x71, 0x09, 0x05, 0x03,   // 7
  0x36, 0x49, 0x49, 0x49, 0x36,   // 8
  0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
  0x00, 0x36, 0x36, 0x00, 0x00,   // :
  0x00, 0x56, 0x36, 0x00, 0x00,   // ;
  0x08, 0x14, 0x22, 0x41, 0x00,   // <
  0x14, 0x14, 0x14, 0x14, 0x14,   // =
  0x00, 0x41, 0x22, 0x14, 0x08,   // >
  0x02, 0x01, 0x51, 0x09, 0x06,   // ?

  0x32, 0x49, 0x59, 0x51, 0x3E,   // @
  0x7C, 0x12, 0x11, 0x12, 0x7C,   // A
  0x7F, 0x49, 0x49, 0x49, 0x36,   // B
  0x3E, 0x41, 0x41, 0x41, 0x22,   // C
  0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
  0x7F, 0x49, 0x49, 0x49, 0x41,   // E
  0x7F, 0x09, 0x09, 0x09, 0x01,   // F
  0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
  0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
  0x00, 0x41, 0x7F, 0x41, 0x00,   // I
  0x20, 0x40, 0x41, 0x3F, 0x01,   // J
  0x7F, 0x08, 0x14, 0x22, 0x41,   // K
  0x7F, 0x40, 0x40, 0x40, 0x40,   // L
  0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
  0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
  0x3E, 0x41, 0x41, 0x41, 0x3E,   // O

  0x7F, 0x09, 0x09, 0x09, 0x06,   // P
  0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
  0x7F, 0x09, 0x19, 0x29, 0x46,   // R
  0x46, 0x49, 0x49, 0x49, 0x31,   // S
  0x01, 0x01, 0x7F, 0x01, 0x01,   // T
  0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
  0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
  0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
  0x63, 0x14, 0x08, 0x14, 0x63,   // X
  0x07, 0x08, 0x70, 0x08, 0x07,   // Y
  0x61, 0x51, 0x49, 0x45, 0x43,   // Z
  0x00, 0x7F, 0x41, 0x41, 0x00,   // [
  0x55, 0xAA, 0x55, 0xAA, 0x55,   // \ Backslash (Checker pattern)
  0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
  0x04, 0x02, 0x01, 0x02, 0x04,   // ^
  0x40, 0x40, 0x40, 0x40, 0x40,   // _

  0x00, 0x03, 0x05, 0x00, 0x00,   // `
  0x20, 0x54, 0x54, 0x54, 0x78,   // a
  0x7F, 0x48, 0x44, 0x44, 0x38,   // b
  0x38, 0x44, 0x44, 0x44, 0x20,   // c
  0x38, 0x44, 0x44, 0x48, 0x7F,   // d
  0x38, 0x54, 0x54, 0x54, 0x18,   // e
  0x08, 0x7E, 0x09, 0x01, 0x02,   // f
  0x18, 0xA4, 0xA4, 0xA4, 0x7C,   // g
  0x7F, 0x08, 0x04, 0x04, 0x78,   // h
  0x00, 0x44, 0x7D, 0x40, 0x00,   // i
  0x40, 0x80, 0x84, 0x7D, 0x00,   // j
  0x7F, 0x10, 0x28, 0x44, 0x00,   // k
  0x00, 0x41, 0x7F, 0x40, 0x00,   // l
  0x7C, 0x04, 0x18, 0x04, 0x78,   // m
  0x7C, 0x08, 0x04, 0x04, 0x78,   // n
  0x38, 0x44, 0x44, 0x44, 0x38,   // o

  0xFC, 0x24, 0x24, 0x24, 0x18,   // p
  0x18, 0x24, 0x24, 0x18, 0xFC,   // q
  0x7C, 0x08, 0x04, 0x04, 0x08,   // r
  0x48, 0x54, 0x54, 0x54, 0x20,   // s
  0x04, 0x3F, 0x44, 0x40, 0x20,   // t
  0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
  0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
  0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
  0x44, 0x28, 0x10, 0x28, 0x44,   // x
  0x1C, 0xA0, 0xA0, 0xA0, 0x7C,   // y
  0x44, 0x64, 0x54, 0x4C, 0x44,   // z
  0x00, 0x10, 0x7C, 0x82, 0x00,   // {
  0x00, 0x00, 0xFF, 0x00, 0x00,   // |
  0x00, 0x82, 0x7C, 0x10, 0x00,   // }
  0x00, 0x06, 0x09, 0x09, 0x06    // ~ (Degrees)
}; // 95


void dispaly_test(void){
  memset(&display_buff_, 0xff, sizeof(display_buff_));
  for (uint16_t i=0; i<8; i++){
    display_update_(i);
  }
}

void display_begin(void){
  SET_DDR_OUT(DISPLAY_DDR, SDA);
  SET_DDR_OUT(DISPLAY_DDR, SCL);
  i2c_init_();

  for (uint8_t i=0; i<sizeof(OLED_INIT_COMMANDS); i++){
    i2c_start_();
    i2c_command_(SSD1306_COMMAND);
    i2c_command_(pgm_read_byte(OLED_INIT_COMMANDS+i));
    i2c_stop_();
  }
  display_clean();
}

void display_clean(void){
  memset(&display_buff_, 0, sizeof(display_buff_));
  for (uint16_t i=0; i<8; i++){
    display_update_(i);
  }
}

void i2c_command_(uint8_t command){
  TWDR = command;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);              // Clear TWINT to proceed
  while ((TWCR & _BV(TWINT)) == 0) {};
}

void i2c_start_(void){
  TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA); // Send START
  while ((TWCR & _BV(TWINT)) == 0) {};                    // Wait for TWI to be ready
  TWDR = SSD1306_ADDR<<1;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);              // Clear TWINT to proceed
  while ((TWCR & _BV(TWINT)) == 0) {};                    // Wait for TWI to be ready
}

void i2c_stop_(void){
  TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);                // send stop
}

void display_update_(uint8_t y){
  i2c_start_();
  i2c_command_(SSD1306_COMMAND);
  i2c_command_(SSD1306_SET_COLUMN_ADDR);
  i2c_command_(0);  // x
  i2c_command_(127);
  i2c_command_(SSD1306_SET_PAGE_ADDR);
  i2c_command_(y);  // y
  i2c_command_(7);
  i2c_stop_();

  i2c_start_();
  i2c_command_(SSD1306_DATA);
  for (uint8_t i=0; i<sizeof(display_buff_); i++){
    TWDR = display_buff_[i];
    TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);              // clear twint to proceed
    while ((TWCR & _BV(TWINT)) == 0) {};                    // wait for twi to be ready
  }
  i2c_stop_();
}

void i2c_init_(void){
  // activate internal pullups for twi.
  SET_HIGH(DISPLAY_PORT, SDA);
  SET_HIGH(DISPLAY_PORT, SCL);

  // initialize twi prescaler and bit rate
  __cbi2(TWSR, TWPS0);
  __cbi2(TWSR, TWPS1);
  TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

  // enable twi module, acks, and twi interrupt
  TWCR = _BV(TWEN) | _BV(TWIE)/* | _BV(TWEA)*/;
}

void print(char* str, uint8_t x, uint8_t y){
  invert_text_ = 0;
  print_string_(str, x, y);
}

void print_invert(char* str, uint8_t x, uint8_t y){
  invert_text_ = 1;
  print_string_(str, x, y);
}

void print_string_(char* str, uint8_t x, uint8_t y){
  if (x > 127 || y > 7){return;}
  memset(&display_buff_, 0, sizeof(display_buff_));
  uint8_t i=0;
  uint8_t buff_offset;
  while(str[i]){
    buff_offset = x + i * (FONT_WIDTH + 1);
    print_char_(str[i], buff_offset);
    i++;
  }
  display_update_(y);
}

void print_char_(uint8_t c, uint8_t buff_offset){
  uint16_t char_offset = (c - FONT_OFFSET)*FONT_WIDTH;
  for (uint8_t i=0; i<FONT_WIDTH; i++){
    if (buff_offset+i >= 127){return;}
    uint8_t b = pgm_read_byte(FONT+char_offset+i);
    if (invert_text_){b = ~b;}
    display_buff_[buff_offset+i] = b;
  }
  if (invert_text_ && buff_offset+FONT_WIDTH <= 127){
    display_buff_[buff_offset+FONT_WIDTH] = 0xff;
  }
}

// FIXME
void clean_buf(void){
  memset(&display_buff_, 0, sizeof(display_buff_));
}

void print_c(uint8_t c, uint8_t x, uint8_t y){
  if (c < 10){c += (16 + FONT_OFFSET);}
  else{c +=  (23 + FONT_OFFSET);}
  print_char_(c, x);
  display_update_(y);
}

void print8(uint8_t n, uint8_t x, uint8_t y){
  print_c(n>>4, x, y);
  print_c(n&0xf, x+8, y);
}

void print16(uint16_t n, uint8_t x, uint8_t y){
  print8(n>>8, x, y);
  print8(n&0xFF, x+16, y);
}

void print32(uint32_t n, uint8_t x, uint8_t y){
  print16(n>>16, x, y);
  print16(n&0xFFFF, x+32, y);
}