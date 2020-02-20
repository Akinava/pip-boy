#include "boot.h"

int main(void){
  setup_button();
  setup_led();

  // MAIN LOGIC
  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    SET_HIGH(LED_PORT, LED_PIN);
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){};
    SET_LOW(LED_PORT, LED_PIN);
    load(BOOT_APP);
  }else{
    goto *start_address;
  }
  return 0;
}

void load(const char* file_path){
  setup_led();
  ADDRESS = 0;

  if (!sd_init()){
    error_light();
  }

  file_t boot_file;
  if (!file_open(file_path, &boot_file)){
    error_blink();
  }

  while(boot_file.cursor < boot_file.size){
    if (!file_read(&boot_file, PAGE_BUFFER, SPM_PAGESIZE)){
      error_blink();
    }
    block_flash_load();
  }
  SET_HIGH(LED_PORT, LED_PIN);
  _delay_ms(100);
  SET_LOW(LED_PORT, LED_PIN);

  reboot();
 }

void block_flash_load(void){
	uint16_t temp_address = ADDRESS;	
	uint16_t i, temp_word;

	// Perform page erase
  //boot_page_erase(address);

	// Wait until the memory is erased
  //boot_spm_busy_wait();		

	// fill the flash page buffer with the data
  for(i = 0; i < SPM_PAGESIZE; i+=2){
		// load the little end byte from the word
		temp_word = PAGE_BUFFER[i];

		// load the big end byte
		temp_word += (PAGE_BUFFER[i+1] << 8);
		
		// put the word into the page butter
    boot_page_fill(ADDRESS, temp_word);;
    	
		// incrememnt the word address						
    ADDRESS += 2;
  } 
    
	// write the page to flash
  boot_page_write(temp_address);

	// wait until finished writing
  boot_spm_busy_wait();
	
	// Re-enable the RWW section 
  //boot_rww_enable();
 
}

void setup_button(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}
void setup_led(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}

void error_light(void){
    SET_HIGH(LED_PORT, LED_PIN);
    while(1){}
}

void error_blink(void){
  while(1){
    TOGGLE(LED_PORT, LED_PIN);
    _delay_ms(1000);
  }
}

void reboot(){
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = WATCHDOG_16MS;
	while (1);
}
