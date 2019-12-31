#include "boot.h"


int main(void){
  // setup led
  SET_DDR_OUT(LED_DDR, LED_PIN);
  // setup button
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);

  //uint8_t size = 16;
	//blockFlashLoad(size);
  displayBegin();

  /*
  myOLED.begin();
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
  */

  while(1){
    if (CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
      TOGGLE(LED_PORT, LED_PIN);
      _delay_ms (1000); // 1 sec
    }else{
    //  SET_LOW(LED_PORT, LED_PIN);
     SET_HIGH(LED_PORT, LED_PIN);
     }
  }
  return 0;
}


/*****************************************************
	Flash Functions
******************************************************/
/*
void blockFlashLoad(uint16_t size)
{ 
	// address is global and set elsewhere
	uint16_t tempAddress = address;	
	uint16_t i,tempWord;

	// Perform page erase
  boot_page_erase(address);

	// Wait until the memory is erased
  boot_spm_busy_wait();		

	// fill the flash page buffer with the data
  for(i = 0; i < size; i+=2){
		// load the little end byte from the word
		tempWord = pageBuffer[i];

		// load the big end byte
		tempWord += (pageBuffer[i+1] << 8);
		
		// put the word into the page butter
    boot_page_fill(address,tempWord);;
    	
		// incrememnt the word address						
    address = address + 2;
  } 
    
	// write the page to flash
  boot_page_write(tempAddress);

	// wait until finished writing
  boot_spm_busy_wait();
	
	// Re-enable the RWW section 
  boot_rww_enable();
}
*/
