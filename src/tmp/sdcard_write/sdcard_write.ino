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
#define PIN_RADIO_CE 0
#define PIN_RADIO_CSN 1


#include <SPI.h>
#include "SdFat.h"
SdFat SD;

void blink();

#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
extern uint8_t TinyFont[];

#define SD_CS_PIN SS
File myFile;

void setup() {
  myOLED.begin();
  myOLED.setFont(TinyFont);

  pinMode(PIN_RADIO_CSN, OUTPUT);
  digitalWrite(PIN_RADIO_CSN, LOW);
  
  pinMode(LED_PIN, OUTPUT);
  
  if (!SD.begin(SD_CS_PIN)) {
    digitalWrite(LED_PIN, HIGH);
    return;
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //myFile = SD.open("test.txt", FILE_WRITE);
  myFile = SD.open("boot.bin");

  // if the file opened okay, write to it:
  if (myFile) {
  //  while (myFile.available()) {
      for (uint8_t x=0; x<15; x++){
        myOLED.print(String(myFile.read(), HEX), x*8, 0);
        myOLED.update();
      }
  //  }
    //myFile.println("testing 1, 2, 3.");
    // close the file:
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
