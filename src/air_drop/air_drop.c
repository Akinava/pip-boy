#include "air_drop.h"

int main(void){
  display_begin();
  display_clean();
  if (radio_init()){
    print("OK", 0, 0);
  }else{
    print("FAIL", 0, 0);
  }
  while(1);
}
