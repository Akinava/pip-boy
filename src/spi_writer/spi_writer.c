#include "spi_writer.h"

int main(void){
  display_begin();
  menu_init();
  menu.event = NOOP;
  menu.cursor = 0;
  menu.page = PAGE_MAIN;
  
  while(1){
    show_menu();
    menu.event = read_key();
    react_event();
  }
}

void show_menu(void){
  if (menu.page == PAGE_MAIN){show_main_menu();}
  if (menu.page == PAGE_LOAD_FUSES){show_load_fuses_menu();}
  if (menu.page == PAGE_FUSES_EDIT){show_fuses_edit();}
  if (menu.page == PAGE_LOAD_APP){show_load_app_menu();}
  if (menu.page == PAGE_APP_ADDR_SET){show_app_addr_set_menu();}
}

void react_event(void){
  // PAGE can be changed during react
  switch(menu.page){
    case PAGE_MAIN:
      react_main_menu();
      break;
    case PAGE_LOAD_FUSES:
      react_load_fuses();
      break;
    case PAGE_FUSES_EDIT:
      react_fuses_edit();
      break;
    case PAGE_FUSES_WRITE:
      react_write_fuses();
      break;
    case PAGE_LOAD_APP:
      react_load_app();
      break;
    case PAGE_APP_ADDR_SET:
      react_app_addr_set();
      break;
    case PAGE_APP_WRITE:
      react_app_write();
      break;
    case PAGE_APP_FILE:
      react_choose_the_file();
      break;
  }
  menu.event = NOOP;
}

void react_choose_the_file(void){
  char file_name_buf[8+1+3+1];
  menu.page = PAGE_LOAD_APP;

  if (!choose_file_menu(&app_file_cluster, &app_file_size, file_name_buf)){
    return;
  }

  // copy file name
  int8_t app_name_cur = 17;
  int8_t file_name_cur = 8+1+3;
  while(app_name_cur > 5){
    if (file_name_cur < 0){
      app_name_buf[app_name_cur] = CHAR_SPACE;
      app_name_cur--;
      continue;  
    }
    if (file_name_buf[file_name_cur] == 0){
      file_name_cur--;
      continue;
    }
    app_name_buf[app_name_cur] = file_name_buf[file_name_cur];
    app_name_cur--;
    file_name_cur--;
  }
}

void react_app_write(void){
  menu.page = PAGE_LOAD_APP;
  // write app
  if (app_file_cluster == 0){
    print("Error: no file", 0, 2);
    _delay_ms(1000);
    return;
  }

  if (program_firmware()){  
    print("DONE", 0, 2);
  }
  // delay for display info
  _delay_ms(1000);
}

void show_app_addr_set_menu(void){
  print("start addr:", 0, 0);
  for (uint8_t i=0; i<4; i++){
    uint8_t half_byte = (app_addr_start>>4*i)&0xf;
    if (i == 3-sub_coursor){
      print4inv(half_byte, (APP_ADDR_INDENT+3-i)*8, 0);
    }else{
      print4(half_byte, (APP_ADDR_INDENT+3-i)*8, 0);
    }  
  }
}

void react_app_addr_set(void){
  if (menu.event == C_KEY_PRESSED){menu.page = PAGE_LOAD_APP;}

  if (menu.event == UP_KEY_PRESSED){up_load_addr();}
  if (menu.event == A_KEY_PRESSED){up_load_addr();}

  if (menu.event == DOWN_KEY_PRESSED){down_load_addr();}
  if (menu.event == B_KEY_PRESSED){down_load_addr();}

  if (menu.event == RIGHT_KEY_PRESSED){sub_coursor++;}
  if (menu.event == LEFT_KEY_PRESSED){sub_coursor--;}
   
  if (sub_coursor < 0){sub_coursor = 0;}
  if (sub_coursor > 2){sub_coursor = 2;}
}

void up_load_addr(void){
  uint16_t diff = 1<<4*(3-sub_coursor);
  if (app_addr_start + diff > signatures[ATMEGA328P].ram_size - signatures[ATMEGA328P].page_size){return;}
  app_addr_start += diff;
}

void down_load_addr(void){
  uint16_t diff = 1<<4*(3-sub_coursor);
  if (app_addr_start < diff){return;}
  app_addr_start -= diff;
}

void show_load_app_menu(void){
  display_clean();
  print("start addr:       ", 0, 0);
  print16(app_addr_start, APP_ADDR_INDENT*8, 0);
  print(app_name_buf, 0, 1);
  print("write             ", 0, 2);

  if (menu.cursor == 0){
    print_invert("start addr:       ", 0, 0);
    print16inv(app_addr_start, APP_ADDR_INDENT*8, 0);
  }

  if (menu.cursor == 1){
    print("start addr:       ", 0, 0);
    print16(app_addr_start, APP_ADDR_INDENT*8, 0);
    print_invert(app_name_buf, 0, 1);
  }

  if (menu.cursor == 2){
    print_invert("write             ", 0, 2);
  }
}

void react_load_app(void){
  if (menu.event == C_KEY_PRESSED){
    menu.cursor = 0;
    menu.page = PAGE_MAIN;
  }
  if (menu.event == UP_KEY_PRESSED){menu.cursor--;}
  if (menu.event == DOWN_KEY_PRESSED){menu.cursor++;}
  if (menu.cursor < 0){menu.cursor = 0;}
  if (menu.cursor > 2){menu.cursor = 2;}
  if (menu.event == A_KEY_PRESSED){
    menu.page = PAGE_APP_ADDR_SET + menu.cursor;
    sub_coursor = 0;
  }
}

void show_load_fuses_menu(void){
  display_clean();
  print("|high| low| ext|", 0, 0);
  if (menu.cursor == 0){
    print_invert("                ", 0, 1);
    for (uint8_t i=0; i<sizeof(write_fuses_command); i++){
      print8inv(signatures[ATMEGA328P].fuses[i], fuses_palce[i]*8, 1);
    }
    print("write", 0, 2);
  }else{
    clean_buf();
    for (uint8_t i=0; i<sizeof(write_fuses_command); i++){
      print8(signatures[ATMEGA328P].fuses[i], fuses_palce[i]*8, 1);
    }
    print_invert("write           ", 0, 2);
  }
}

void react_load_fuses(void){
  if (menu.event == C_KEY_PRESSED){
    menu.cursor = 1;
    menu.page = PAGE_MAIN;
  }
  if (menu.event == UP_KEY_PRESSED){menu.cursor--;}
  if (menu.event == DOWN_KEY_PRESSED){menu.cursor++;}
  if (menu.cursor < 0){menu.cursor = 0;}
  if (menu.cursor > 1){menu.cursor = 1;}
  if (menu.event == A_KEY_PRESSED){
    sub_coursor = 0;
    menu.page = PAGE_FUSES_EDIT + menu.cursor;
  }
}

void show_main_menu(void){
  display_clean();
  for (uint8_t i=0; i<MAX_LINES; i++){
    if (i==menu.cursor){
      print_invert(MENU_LIST[i], 0, i);
    }else{
      print(MENU_LIST[i], 0, i);
    }
  }
}

void react_main_menu(void){
  if (menu.event == UP_KEY_PRESSED){menu.cursor--;}
  if (menu.event == DOWN_KEY_PRESSED){menu.cursor++;}
  if (menu.cursor == -1){menu.cursor = 0;}
  if (menu.cursor == MAX_LINES){menu.cursor = MAX_LINES - 1;}
  if (menu.event == A_KEY_PRESSED){
    menu.page = menu.cursor;
    menu.cursor = 0;
  }
  if (menu.event == C_KEY_PRESSED){app_exit();}

}

uint8_t init_program_mode(uint8_t chip){
  if(!program_enable()){
    print("no responce from chip", 0, 2);
    _delay_ms(1000);
    spi_deactivate();
    return 0;
  }
  if (!read_signature(chip)){
    print("wrong signarure", 0, 2);
    _delay_ms(1000);
    spi_deactivate();
    return 0;
  }
  return 1;
}

void react_write_fuses(void){
  menu.page = PAGE_LOAD_FUSES;
  spi_activate();
  if (!init_program_mode(ATMEGA328P)){
    return;
  }

  for (uint8_t i=0; i<sizeof(write_fuses_command); i++){
    write_fuse(write_fuses_command[i], signatures[ATMEGA328P].fuses[i]);
  }
  spi_deactivate();
  print("DONE", 0, 2);
  _delay_ms(1000);
}

void show_fuses_edit(void){
  print("                ", 0, 1);
  for (uint8_t i=0; i<sizeof(fuses_palce); i++){
    if (sub_coursor/2 == i){
      if (sub_coursor%2 == 0){
        print4inv(signatures[ATMEGA328P].fuses[i]>>4, fuses_palce[i]*8, 1);
        print4(signatures[ATMEGA328P].fuses[i]&0xf, fuses_palce[i]*8+8, 1);
      }else{
        print4(signatures[ATMEGA328P].fuses[i]>>4, fuses_palce[i]*8, 1);
        print4inv(signatures[ATMEGA328P].fuses[i]&0xf, fuses_palce[i]*8+8, 1);
      }
    }else{
      print8(signatures[ATMEGA328P].fuses[i], fuses_palce[i]*8, 1);
    }
  }
}

void react_fuses_edit(void){
  if (menu.event == C_KEY_PRESSED){menu.page = PAGE_LOAD_FUSES;}

  if (menu.event == UP_KEY_PRESSED){up_fuse();}
  if (menu.event == A_KEY_PRESSED){up_fuse();}

  if (menu.event == DOWN_KEY_PRESSED){down_fuse();}
  if (menu.event == B_KEY_PRESSED){down_fuse();}

  if (menu.event == RIGHT_KEY_PRESSED){sub_coursor++;}
  if (menu.event == LEFT_KEY_PRESSED){sub_coursor--;}
   
  if (sub_coursor < 0){sub_coursor = 0;}
  if (sub_coursor > sizeof(fuses_palce)*2-1){sub_coursor = sizeof(fuses_palce)*2-1;}
}

void up_fuse(void){
  uint8_t fuse = signatures[ATMEGA328P].fuses[sub_coursor/2];
  if (sub_coursor%2 == 0){
    if (fuse+0x10<=0xff){fuse+=0x10;}
  }else{
    if (fuse+1<=0xff){fuse+=1;}
  }
  signatures[ATMEGA328P].fuses[sub_coursor/2] = fuse;
}

void down_fuse(void){
  uint8_t fuse = signatures[ATMEGA328P].fuses[sub_coursor/2];
  if (sub_coursor%2 == 0){
    if (fuse>=0x10){fuse-=0x10;}
  }else{
    if (fuse>=1){fuse-=1;}
  }
  signatures[ATMEGA328P].fuses[sub_coursor/2] = fuse;
}

void app_exit(void){
  display_clean();
  print("DON'T TURN OFF!", 0, 0);
  load_default_app();
}

/********************************* ISP ***************************************/

void spi_activate(void){
  // Set MOSI, SCK, SD_CS as Output B
  SPI_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // set MISO as Input
  SET_DDR_IN(SPI_DDR, MISO);
  // Set RADIO_CS, SPI_MASTER as Output D
  RADIO_DDR |= _BV(RADIO_CSN)|_BV(SPI_MASTER_PIN);

  // Enable SPI, Set as Master and Prescaler Fosc/4, by default
  SPCR = _BV(SPE)|_BV(MSTR);
  //SET_LOW(SPSR, SPI2X);  
  //SET_LOW(SPI_MASTER_PORT, SPI_MASTER_PIN);
  //Prescaler: Fosc/16
  //SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0);

  // off other spi hw and radio
  SPI_UNSET(SD_PORT, SD_CS);
  SPI_UNSET(RADIO_PORT, RADIO_CSN);

  // PULSE
  SET_LOW(SPI_PORT, SCK);
  SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  _delay_ms(10);
  SPI_SET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  //_delay_ms(25);
}

void spi_deactivate(void){
  SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  SET_LOW(SPI_PORT, SCK);
  SET_LOW(SPI_PORT, MOSI);
}

void spi_send(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

uint8_t program_enable(void){
  spi_send(PROGRAM_ENABLE);
  spi_send(PROGRAM_ACKNOWLEDGE);
  spi_send(0x00);
  if (SPDR != PROGRAM_ACKNOWLEDGE){
    return 0;
  }
  spi_send(0x00);
  return 1;
}

uint8_t read_signature(uint8_t chip){
  for (uint8_t i=0; i<3; i++){
    isp_command(READ_SIGNATURE, 0, i, 0);
    if (SPDR != signatures[chip].sig[i]){
      return 0;
    }
  }
  return 1;
}

void erise_chip(void){
  isp_command(PROGRAM_ENABLE, CHIP_ERASE, 0, 0);
  _delay_ms(20);
  busy_wait();
}

void write_fuse(uint8_t fuse, uint8_t value){
  isp_command(PROGRAM_ENABLE, fuse, 0, value);
}

uint8_t program_firmware(void){
  spi_activate();
  if (!init_program_mode(ATMEGA328P)){
    return 0;
  }
  erise_chip();

  uint32_t sector = get_sector_by_cluster_(app_file_cluster);
  uint16_t address = app_addr_start;
  uint16_t sector_offset = SECTOR_SIZE;
  uint8_t page_cursor = 0;

  while(1){
    uint8_t low_byte = 0xff;
    uint8_t high_byte = 0xff;

    if (sector_offset == SECTOR_SIZE){
      SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);
      if (!read_sector_(sector)){
        print("Error: can't read file", 0, 2);
        return 0;
      }
      SPI_SET(SPI_MASTER_PORT, SPI_MASTER_PIN);
      program_enable();
      sector++;
      sector_offset = 0;
    }
    if ((address-app_addr_start) < app_file_size){
      low_byte = sector_buffer[sector_offset];
      high_byte = sector_buffer[sector_offset+1];
    }
    isp_command(LOAD_PROGRAM_LOW_BYTE, 0, page_cursor/2, low_byte);
    isp_command(LOAD_PROGRAM_HIGH_BYTE, 0, page_cursor/2, high_byte);

    sector_offset += 2;
    page_cursor += 2;

    if (page_cursor == signatures[ATMEGA328P].page_size){
      page_cursor = 0;
      isp_command(
          WRITE_PAGE,
          (address/2)>>8,
          (address/2)&0xff,
          0);
      busy_wait();
      address += signatures[ATMEGA328P].page_size;
      if ((address-app_addr_start) >= app_file_size){break;}
    }
  }
  spi_deactivate();


  /*
  for (uint8_t i=0; i<64; i++){
    isp_command(LOAD_PROGRAM_LOW_BYTE, 0, i, 0);
    isp_command(LOAD_PROGRAM_HIGH_BYTE, 0, i, 0);
  }
  isp_command(WRITE_PAGE, 0, 0, 0);
  busy_wait();

  SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  
  read_sector_(0);
  //_delay_ms(100);

  SPI_SET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  program_enable();

  // second page
  for (uint8_t i=0; i<64; i++){
    isp_command(LOAD_PROGRAM_LOW_BYTE, 0, i, 0);
    isp_command(LOAD_PROGRAM_HIGH_BYTE, 0, i, 0);
  }
  isp_command(WRITE_PAGE, 0, 64, 0);
  busy_wait();

  spi_deactivate();
  */
  //SPI_UNSET(SPI_MASTER_PORT, SPI_MASTER_PIN);
  //SPI_SET(SPI_MASTER_PORT, SPI_MASTER_PIN);

  // the issue with reset blink
  // it nessesaru onli one time
  // with set and unset we neet also 
  // setup a speed

  // program
  // app_addr_start
  // app_file_cluster
  // app_file_size
  /*
  //uint32_t sector = get_sector_by_cluster_(app_file_cluster);
  uint16_t address = app_addr_start;
  uint8_t page_cursor = signatures[ATMEGA328P].page_size;
  uint16_t sector_offset = SECTOR_SIZE;

  while(1){
    uint8_t low_byte = 0xff;
    uint8_t high_byte = 0xff;

    if (sector_offset == SECTOR_SIZE){
      
      spi_deactivate();
      
      if (!read_sector_(sector)){
        print("Error: can't read file", 0, 2);
        return 0;
      }
      
      sector++;
      sector_offset = 0;
       
      if (!init_program_mode(ATMEGA328P)){
        return 0;
      }
    }
    
    // read bytes
    if ((address-app_addr_start) < app_file_size){
      low_byte = sector_buffer[sector_offset];
      high_byte = sector_buffer[sector_offset+1];
    }
    
    // write bytes
    isp_command(LOAD_PROGRAM_LOW_BYTE, 0, page_cursor/2, low_byte);
    isp_command(LOAD_PROGRAM_HIGH_BYTE, 0, page_cursor/2, high_byte);

    address += 2;
    sector_offset += 2;
    page_cursor -= 2;

    // write page
    if (page_cursor == 0){
      page_cursor = signatures[ATMEGA328P].page_size;
      isp_command(
          WRITE_PAGE,
          0, // FIXME
          0, // FIXME
          0);
      busy_wait();
      break;
      // check if it is done
      if ((address-app_addr_start) >= app_file_size){break;}
    }
  }
  */

  return 1;
}

void busy_wait(void){
  do{
    isp_command(POOL_READY, 0, 0, 0);
  }while((SPDR & 1) == 1);
}

void isp_command(uint8_t cmd0, uint8_t cmd1, uint8_t cmd2, uint8_t cmd3){
  spi_send(cmd0);
  spi_send(cmd1);
  spi_send(cmd2);
  spi_send(cmd3);
}

