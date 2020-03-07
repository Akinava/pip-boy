/*
 * Example use of chdir(), ls(), mkdir(), and  rmdir().
 */
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
extern uint8_t SmallFont[];

#include <SPI.h>
#include "SdFat.h"

// SD card chip select pin.
const uint8_t chipSelect = SS;

// disable NRF24L01
#define NRF24L01_CS 0
//------------------------------------------------------------------------------

// File system object.
SdFat sd;

//// Directory file.
SdFile dir;
SdFile sub_dir;

// Use for file creation in folders.
SdFile file;


//==============================================================================
// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))
//------------------------------------------------------------------------------
void setup() {
  pinMode(NRF24L01_CS, OUTPUT);
  digitalWrite(NRF24L01_CS, HIGH);
  
  myOLED.begin();
  myOLED.setFont(SmallFont);
  
  /*
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    myOLED.clrScr();
    myOLED.print("SD card problem", CENTER, 16);
    myOLED.update();
    sd.initErrorHalt();
  }
    
  int rootFileCount = 0;
  if (!dir.open("/")) {
    error("open root failed");
  }
  myOLED.clrScr();
  myOLED.print("read root", CENTER, 16);
  myOLED.update();
  */

  // x 21 rows
  // y  8 lines
  /*    
  myOLED.clrScr();
  myOLED.print("123456789012345678901", 0, 0);
  for (uint8_t i=2; i<9; i++){
    myOLED.print(String(i), 0, (i-1)*8);
  }
  myOLED.update();
  */
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    myOLED.print("SD init error.", CENTER, 16);
    myOLED.update();
    while(1);
  }
  if (!sd.chdir()) {
    myOLED.print("chdir to root failed.", CENTER, 16);
    myOLED.update();
    while(1);
  }
  if (!dir.open("/")) {
    myOLED.print("open root failed.", CENTER, 16);
    myOLED.update();
    while(1);
  }

}
//------------------------------------------------------------------------------
// Nothing happens in loop.
void loop() {
  // if (!root.open("/")) {};
  //if (file.isDir()) {};
  //if (!sd.chdir("Folder1")) {};
  //sd.ls(LS_R);
  char name[13];
  for (uint8_t i=0;i<8;i++){
    if (!sub_dir.openNext(&dir, O_RDONLY)){break;}
    sub_dir.getFilename(name);
    myOLED.print(name, CENTER, 16);
    myOLED.update();
  }
  while(1);
  
  
}
