/*
  SD card file dump

 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.

 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created  22 December 2010
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

/*
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
extern uint8_t SmallFont[];
*/

#define LED_PIN 5

#include <SPI.h>
#include <SD.h>

/*
void oled_print(char* text){
  myOLED.clrScr();
  myOLED.update();
  myOLED.print(text, 0, 0);
  myOLED.update();
  delay(2000);
}
*/


void error(){
  while (1){
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500);                       // wait for a second
  }
}

void setup() {
  /*
  myOLED.begin();
  myOLED.setFont(SmallFont);
  oled_print((char*)"Init SD card");
  */

  pinMode(LED_PIN, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(10)) {

    //oled_print((char*)"Card failed");
    // don't do anything more:
    error();
  }

  //oled_print((char*)"card initialized");

  // write
  File dataFile_w = SD.open("test.txt", FILE_WRITE);

  //oled_print((char*)"open file");
  
  const String dataString = "abcd";
  if (dataFile_w){
    dataFile_w.println("testing 1, 2, 3.");
    dataFile_w.close();

    //oled_print((char*)"write file");
  }else{
    //oled_print((char*)"error write file");
    //while (1);
    error();
  }

  /*
  // read
  File dataFile_r = SD.open("datalog.txt");
  if (dataFile) {
    oled_print((char*)dataFile_r.read());
    dataFile_r.close();
  }else{
    oled_print((char*)"error read datalog.txt");
  }*/
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
}
