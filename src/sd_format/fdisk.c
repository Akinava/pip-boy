#include "fdisk.h"

int main(void){
  display_begin();
  display_clean();

  if (!card_init_()){
    print("SD read fail", 26, 3);
    while(1);
  }
  uint32_t card_size;
  if (get_card_size(&card_size)){
    print32(card_size, 0, 4);
  }
  print("DONE", 50, 3);
  while(1);
}
