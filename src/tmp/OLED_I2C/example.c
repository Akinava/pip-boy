#include "example.h"

int main(void){
  displayBegin();
  displayClean();
  //uint8_t st[] = {'T', 'i', 'm', 'a', 0};
  uint8_t st[] = "Hello my dear";
  uint8_t st1[] = "hospital!";
  print(st, 0, 0);
  print(st1, 0, 10);
  displayUpdate();
  while(1);
}
