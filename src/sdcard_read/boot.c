#include "boot.h"


int main(void){
  // setup led
  SET_DDR_OUT(LED_DDR, LED_PIN);
  // setup button
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
  SET_LOW(LED_PORT, LED_PIN);

  while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    SET_HIGH(LED_PORT, LED_PIN);
  }

  displayBegin();
  displayClean();


  //for (uint8_t i=0; i<64; i++){
  //  displayPrintHex(0, i%16, i/16);
  //}
  //displayUpdate();
 
  if (!sd_begin()){
    SET_HIGH(LED_PORT, LED_PIN);
  }


  //fileOpen();
  //uint8_t data = fileRead();
  //fileRead();
  //fileClose();
  //sdStop();

  while(1){
     //SET_HIGH(LED_PORT, LED_PIN);
  }
  return 0;
}
