#include "display.h"

const uint8_t SmallFont[] PROGMEM = {
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

void displayBegin(void){
  SET_DDR_OUT(DISPLAY_DDR, SDA);
  SET_DDR_OUT(DISPLAY_DDR, SCL);
  _initTWI();
  _sendTWIcommand(SSD1306_DISPLAY_OFF);
  _sendTWIcommand(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
  _sendTWIcommand(0x80);
  _sendTWIcommand(SSD1306_SET_MULTIPLEX_RATIO);
  _sendTWIcommand(0x3F);
  _sendTWIcommand(SSD1306_SET_DISPLAY_OFFSET);
  _sendTWIcommand(0x0);
  _sendTWIcommand(SSD1306_SET_START_LINE | 0x0);
  _sendTWIcommand(SSD1306_CHARGE_PUMP);
  _sendTWIcommand(0x14);
  _sendTWIcommand(SSD1306_MEMORY_ADDR_MODE);
  _sendTWIcommand(0x00);
  _sendTWIcommand(SSD1306_SET_SEGMENT_REMAP | 0x1);
  _sendTWIcommand(SSD1306_COM_SCAN_DIR_DEC);
  _sendTWIcommand(SSD1306_SET_COM_PINS);
  _sendTWIcommand(0x12);
  _sendTWIcommand(SSD1306_SET_CONTRAST_CONTROL);
  _sendTWIcommand(0xCF);
  _sendTWIcommand(SSD1306_SET_PRECHARGE_PERIOD);
  _sendTWIcommand(0xF1);
  _sendTWIcommand(SSD1306_SET_VCOM_DESELECT);
  _sendTWIcommand(0x40);
  _sendTWIcommand(SSD1306_DISPLAY_ALL_ON_RESUME);
  _sendTWIcommand(SSD1306_NORMAL_DISPLAY);
  _sendTWIcommand(SSD1306_DISPLAY_ON);

  displayClean();
  displayUpdate();
}


void displayClean(void){
  for (uint16_t i=0; i<1024; i++)
    scrbuf[i] = 0;
}


void displayUpdate(void){
  //noInterrupts();
  _sendTWIcommand(SSD1306_SET_COLUMN_ADDR);
  _sendTWIcommand(0);
  _sendTWIcommand(127);

  _sendTWIcommand(SSD1306_SET_PAGE_ADDR);
  _sendTWIcommand(0);
  _sendTWIcommand(7);
  
  // Send TWI Start
  // Send start address
  TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
  while ((TWCR & _BV(TWINT)) == 0) {};
  TWDR = SSD1306_ADDR<<1;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);
  while ((TWCR & _BV(TWINT)) == 0) {};
  TWDR = SSD1306_DATA_CONTINUE;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);
  while ((TWCR & _BV(TWINT)) == 0) {};

  for (int b=0; b<1024; b++){    // Send data
    TWDR = scrbuf[b];
    TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);              // Clear TWINT to proceed
    while ((TWCR & _BV(TWINT)) == 0) {};                    // Wait for TWI to be ready
  }

  // Send TWI Stop
  TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);                // Send STOP
  //interrupts();
}


void _initTWI(void){
  // activate internal pullups for twi.
  SET_HIGH(DISPLAY_PORT, SDA);
  SET_HIGH(DISPLAY_PORT, SCL);

  //delay(1);  // Workaround for a linker bug

  // initialize twi prescaler and bit rate
  __cbi2(TWSR, TWPS0);
  __cbi2(TWSR, TWPS1);
  TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

  // enable twi module, acks, and twi interrupt
  TWCR = _BV(TWEN) | _BV(TWIE)/* | _BV(TWEA)*/;
}


void _sendTWIcommand(uint8_t value){
  // Send start address
  TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA); // Send START
  while ((TWCR & _BV(TWINT)) == 0) {};                    // Wait for TWI to be ready
  TWDR = SSD1306_ADDR<<1;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);              // Clear TWINT to proceed
  while ((TWCR & _BV(TWINT)) == 0) {};                    // Wait for TWI to be ready

  TWDR = SSD1306_COMMAND;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);              // Clear TWINT to proceed
  while ((TWCR & _BV(TWINT)) == 0) {};                    // Wait for TWI to be ready
  TWDR = value;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);              // Clear TWINT to proceed
  while ((TWCR & _BV(TWINT)) == 0) {};                    // Wait for TWI to be ready

  TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);              // Send STOP
}

void print_char(uint8_t c, uint8_t x, uint8_t y){
  invert_text_ = 0;
  print_char_(c, x, y);
}

void print_invert_char(uint8_t c, uint8_t x, uint8_t y){
  invert_text_ = 1;
  print_char_(c, x, y);
}

void print_char_(uint8_t c, uint8_t x, uint8_t y){
  if (x > 127 || y > 63 || c < FONT_OFFSET){return;}
  uint16_t c_offset = (c - FONT_OFFSET)*FONT_WIDTH;
  for (uint8_t i=0; i<FONT_WIDTH; i++){
    if (x+i > 127){break;}
    uint8_t b = pgm_read_byte(SmallFont+c_offset+i);
    if (invert_text_){b = ~b;}
      scrbuf[(y/8)*128+x+i] = b << (y % FONT_HEIGHT);
    if (y%FONT_HEIGHT){
      // print bottom part of char
      scrbuf[(y/8+1)*128+x+i] = b >> (FONT_HEIGHT - y % FONT_HEIGHT);
    }
  }
  if (invert_text_ && x+FONT_WIDTH < 127){
    scrbuf[(y/8)*128+x+FONT_WIDTH] = 0xff << (y % FONT_HEIGHT);
    if (y%FONT_HEIGHT){
      scrbuf[(y/8+1)*128+x+FONT_WIDTH] = 0xff >> (FONT_HEIGHT - y % FONT_HEIGHT);
    }
  }
}

void print(uint8_t* str, uint8_t x, uint8_t y){
  invert_text_ = 0;
  print_string_(str, x, y);
}

void print_invert(uint8_t* str, uint8_t x, uint8_t y){
  invert_text_ = 1;
  print_string_(str, x, y);
}

void print_string_(uint8_t* str, uint8_t x, uint8_t y){
  uint8_t i=0;
  while(str[i]){
    print_char_(str[i], x+i*(FONT_WIDTH+1), y);
    i++;
  }
}
