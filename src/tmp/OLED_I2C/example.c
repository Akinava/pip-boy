#include "example.h"

int main(void){
  displayBegin();
  for (uint8_t i=64; i<95; i++){
    displayClean();
    //uint8_t st[] = {'T', 'i', 'm', 'a', 0};

    print_char(i+32, 0, 0);
    displayUpdate();
    _delay_ms(1000);
  }
  //displayClean();
  //print_char('T', 0, 0);
  //displayUpdate();
  while(1);
}
