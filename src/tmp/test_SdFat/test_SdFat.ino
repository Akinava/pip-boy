const int chipSelect = SS;

#include <SdFat.h>
SdFat sd;
SdFile myFile;

void setup() {
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();

  // open the file for write at end like the Native SD library
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
  }
  
  myFile.println("testing 1, 2, 3.");
  myFile.close();

}

void loop() {
  // nothing happens after setup
}
