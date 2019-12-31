#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);

//extern uint8_t SmallFont[];
extern uint8_t TinyFont[];


void setup()
{
  myOLED.begin();
  //myOLED.setFont(SmallFont);
  myOLED.setFont(TinyFont);
  myOLED.clrScr();
  myOLED.print("00112233445566778899AABBCCDDEEFF", 0, 0);
  myOLED.print("8", 0, 8);
  myOLED.print("16", 0, 16);
  myOLED.print("24", 0, 24);
  myOLED.print("32", 0, 32);
  myOLED.print("40", 0, 40);
  myOLED.print("48", 0, 48);
  myOLED.print("56", 0, 56);
  myOLED.update();
}

void loop()
{
  

//  myOLED.print("Special:", LEFT, 0);
//  myOLED.print("!\"#$%&'()*+,-.", CENTER, 16);
//  myOLED.print("/:;<=>?@[\\]^_`", CENTER, 24);
//  myOLED.print("{|}~", CENTER, 32);
//  myOLED.update();
//  delay (5000);
}
