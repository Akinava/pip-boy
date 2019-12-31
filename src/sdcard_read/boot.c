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
  displayClean();
  displayPrintHex(0x00, 0, 0);
  displayPrintHex(0x15, 15, 0);
  displayPrintHex(0x07, 0, 7);
  displayUpdate();

  while(1){
    if (CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
      TOGGLE(LED_PORT, LED_PIN);
      _delay_ms (1000); // 1 sec
    }else{
     SET_LOW(LED_PORT, LED_PIN);
     //SET_HIGH(LED_PORT, LED_PIN);
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
