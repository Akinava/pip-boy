/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
#define LED_PIN 5
#define BTN_C_PIN 17
#define PIN_RADIO_CE 0
#define PIN_RADIO_CSN 1


#include <SPI.h>
#include "SdFat.h"
SdFat SD;

void blink();

#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
extern uint8_t SmallFont[];

#define SD_CS_PIN SS
File myFile;

String getHex(uint8_t h);
uint8_t c;

void setup() {
  myOLED.begin();
  myOLED.setFont(SmallFont);

  pinMode(PIN_RADIO_CSN, OUTPUT);
  digitalWrite(PIN_RADIO_CSN, HIGH);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_C_PIN, INPUT_PULLUP);

  
  if (!SD.begin(SD_CS_PIN)) {
    digitalWrite(LED_PIN, HIGH);
    return;
  }

  //myFile = SD.open("test.txt", FILE_WRITE);
  myFile = SD.open("boot.bin");


  if (myFile) {
    while (myFile.available()){
    
      for (uint8_t w=0; w<64; w++){
        if (myFile.available())
          c=myFile.read();
        else
          c=0xff;

        myOLED.print(getHex(c), w%8*12, w/8*8);        
      }
      myOLED.update();
      
      while(digitalRead(BTN_C_PIN) == HIGH){}
      delay(1000);

    }
    myFile.close();
  }else{
    blink();
  }
  
}


void loop() {
  // nothing happens after setup
}

void blink() {
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                    // wait
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
}

String getHex(uint8_t h){
   if (h<0x10)
     return "0"+String(h, HEX);
   else 
     return String(h, HEX);
}
