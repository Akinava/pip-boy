#include "boot.h"

int main(void){
  setup_button_();
  setup_led_();

  // MAIN LOGIC
  if(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){
    SET_HIGH(LED_PORT, LED_PIN);
    while(CHECK_PIN(BUTTON_C_PINS, BUTTON_C_PIN)){};
    SET_LOW(LED_PORT, LED_PIN);
    load(BOOT_APP);
  }else{
    app_start();
  }
}

void load(const char* file_path){
  setup_led_();

  uint32_t address = 0;

  if (!sd_init()){
    error_light_();
  }

  file_t boot_file;
  if (!file_open(file_path, &boot_file)){
    error_blink_();
  }

  while(boot_file.cursor < boot_file.size){
    if (!file_read_sector(&boot_file)){
      error_blink_();
    }
    for (uint8_t page=0; page<vol_info.bytes_per_sector/SPM_PAGESIZE; page++){
      block_flash_load_(&address, page);
      if (address + SPM_PAGESIZE*page > boot_file.size){break;}
    }
  }
  SET_HIGH(LED_PORT, LED_PIN);
  _delay_ms(100);
  SET_LOW(LED_PORT, LED_PIN);

  reboot_();
 }

void block_flash_load_(uint32_t* address, uint8_t page){
	// Perform page erase
  //boot_page_erase(address);
	// Wait until the memory is erased
  //boot_spm_busy_wait();		

  for(uint8_t i = 0; i < SPM_PAGESIZE; i+=2){
		uint16_t temp_word = *((uint16_t*)(sector_buffer + i + page * SPM_PAGESIZE));
    boot_page_fill(*address + i, temp_word);;
  } 
  boot_page_write(*address);
	// wait until finished writing
  boot_spm_busy_wait();
	// Re-enable the RWW section 
  //boot_rww_enable();
  *address += SPM_PAGESIZE;
}

void setup_button_(void){
  SET_DDR_IN(BUTTON_C_DDR, BUTTON_C_PIN);
  SET_PULLUP(BUTTON_C_PORT, BUTTON_C_PIN);
}

void setup_led_(void){
  SET_DDR_OUT(LED_DDR, LED_PIN);
  SET_LOW(LED_PORT, LED_PIN);
}

void error_light_(void){
    SET_HIGH(LED_PORT, LED_PIN);
    while(1){}
}

void error_blink_(void){
  while(1){
    TOGGLE(LED_PORT, LED_PIN);
    _delay_ms(1000);
  }
}

void reboot_(){
  WDTCSR = _BV(WDE);
	while (1);
}
