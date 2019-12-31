#include "display.h"


//const uint8_t FontHex[] PROGMEM = {
const uint8_t FontHex[] = {
0x7f, 0x41, 0x7f, 0x00, 0x00, 0x7f, 0x79, 0x49, 0x4f, 0x49, 0x49, 0x7f,  // 012 3
0x0f, 0x08, 0x7f, 0x4f, 0x49, 0x79, 0x7f, 0x49, 0x79, 0x01, 0x01, 0x7f,  // 4567
0x7f, 0x49, 0x7f, 0x0f, 0x09, 0x7f, 0x7c, 0x0b, 0x7c, 0x7f, 0x49, 0x36,  // 89AB
0x3e, 0x41, 0x41, 0x7f, 0x41, 0x3e, 0x7f, 0x49, 0x49, 0x7f, 0x09, 0x09,  // CDEF
};


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


void displayPrintHex(uint8_t h, uint8_t x, uint8_t y){
  _displayPrintHexNum(h>>4, x*8, y);    // print high
  _displayPrintHexNum(h&0xf, x*8+4, y); // print low
}


void _displayPrintHexNum(uint8_t h, uint8_t x, uint8_t y){
  // max x 15 places y 7 lines
  // symbol size 4 8 pix

  for (uint8_t i=0; i<3; i++){
     scrbuf[y*128+x+i] = FontHex[h*3+i];
  }
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
