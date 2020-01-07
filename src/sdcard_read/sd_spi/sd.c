#include "sd.h"

uint8_t sdBegin(void){
  partialBlockRead_ = 0;
  inBlock_ = 0;
  type_ = 0;

  if (!card_init()){return 0;}
  if (!vol_init()){return 0;}
  return 1;
}


uint8_t fileOpen(void){
  return 0;
}

uint8_t fileAvailable(void){
  return 0;
}

uint8_t fileRead(void){
  return 0;
}


void fileClose(void){

}

void sdStop(void){
}

uint8_t vol_init(void){
  uint8_t part = 1; // 0, 1
  uint32_t volumeStartBlock = 0;
  uint8_t buf[BPB_COUNT];
  if (!cdRawRead(volumeStartBlock, PART_OFFSET + 16*(part-1), buf, 16)){return 0;}

  displayBegin();
  displayClean();
  for (uint8_t i=0; i<BPB_COUNT;i++)
    displayPrintHex(buf[i], i%16, i/16);
  displayUpdate();

  // part 0
  // buf 00000000000000000000000000000000
  //     1fd33c8776f79bab01b708ef02f3c919
  //     bf0249c0ff
  //
  // part 1
  // buf 008203000cfeffff002000000060cd01
  //     1dd37c8776f79bab01b708ef02f3c919
  //     bf0249c0ff
  //
  //     16 bytesPerSector 512-0x0200 130-0x0082
  //     8 sectorsPerCluster 03
  //     16 reservedSectorCount
  //     8  fatCount
  //     16 rootDirEntryCount
  //     16 totalSectors16
  //     8  mediaType
  //     16 sectorsPerFat16
  //     16 sectorsPerTrtack
  //     16 headCount
  //     32 hidddenSectors
  //     32 totalSectors32
  //     32 sectorsPerFat32
  //     16 fat32Flags
  //     16 fat32Version
  //     32 fat32RootCluster
  //     16 fat32FSInfo
  //     16 fat32BackBootBlock
  //     8  fat32Reserved[12]
  

  bpb_t *bpb = (bpb_t *)buf;

  if (bpb->bytesPerSector != 512 ||
    bpb->fatCount == 0 ||
    bpb->reservedSectorCount == 0 ||
    bpb->sectorsPerCluster == 0 ||
    (bpb->sectorsPerCluster & (bpb->sectorsPerCluster - 1)) != 0) {
       // not valid FAT volume
       //error(6);
       return 0;
  }
  fatCount_ = bpb->fatCount;
  blocksPerCluster_ = bpb->sectorsPerCluster;
  blocksPerFat_ = bpb->sectorsPerFat16 ? bpb->sectorsPerFat16 : bpb->sectorsPerFat32;
  rootDirEntryCount_ = bpb->rootDirEntryCount;
  fatStartBlock_ = volumeStartBlock + bpb->reservedSectorCount;
  rootDirStart_ = fatStartBlock_ + bpb->fatCount*blocksPerFat_;
  dataStartBlock_ = rootDirStart_ + ((32*bpb->rootDirEntryCount + 511)/512);
  totalBlocks_ = bpb->totalSectors16 ? bpb->totalSectors16 : bpb->totalSectors32;
  clusterCount_ = (totalBlocks_ - (dataStartBlock_ - volumeStartBlock))
                  /bpb->sectorsPerCluster;
  if (clusterCount_ < 4085) {
    fatType_ = 12;
  }
  else if (clusterCount_ < 65525) {
    fatType_ = 16;
  }
  else {
    rootDirStart_ = bpb->fat32RootCluster;
    fatType_ = 32;
  }

  displayBegin();
  displayClean();
  displayPrintHex(fatType_, 0, 0);
  displayUpdate();
 
  return 1;
}

uint8_t card_init(void){
  uint8_t ocr[4];
  uint8_t slow = 0;
  // Set MOSI, SCK, CS as Output
  SD_DDR |= _BV(MOSI)|_BV(SCK)|_BV(SD_CS);
  // set sd cs off
  SET_HIGH(SD_PORT, SD_CS);
  // Enable SPI, Set as Master
  //Prescaler: Fosc/16, Enable Interrupts
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0); // | (1 << SPR1)
  uint8_t r = cardCommand(CMD0, 0, 0X95);  // 0x3f

  for (uint16_t retry = 0; r != R1_IDLE_STATE; retry++){
    if (retry == 0XFFFF) {
      // ERROR 0
      error(0);
      return 0;
    }
    r = spiRec();
  }
  r = cardCommand(CMD8, 0x1AA, 0X87);
  if (r != 1){
    error(1);
    return 0;
  }
  type_ = SD_CARD_TYPE_SD2; 


  for (uint16_t retry = 0; ; retry++) {
    cardCommand(CMD55, 0, 0XFF);
    r = cardCommand(ACMD41, type_ == SD_CARD_TYPE_SD2 ? 0X40000000 : 0, 0XFF);
    if (r == R1_READY_STATE)break;
    if (retry == 1000) {
      error(2);
      return 0;
    }
  }
  if(cardCommand(CMD58, 0, 0XFF)) {
      error(3);
      return 0;
  }
  for (uint8_t i = 0; i < 4; i++) ocr[i] = spiRec();
  if (type_ == SD_CARD_TYPE_SD2 && (ocr[0] & 0XC0) == 0xC0) type_ = SD_CARD_TYPE_SDHC;
  //use max SPI frequency
  SPCR &= ~((1 << SPR1) | (1 << SPR0)); // f_OSC/4
  if (!slow) SPSR |= (1 << SPI2X); // Doubled Clock Frequency: f_OSC/2
  SET_HIGH(SD_PORT, SD_CS);
  
  return 1;
}

void spiSend(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
}

uint8_t spiRec(void){
  SPDR = 0xFF;
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}


uint8_t cardCommand(uint8_t cmd, uint32_t arg, uint8_t crc){
  uint8_t r1;
  // end read if in partialBlockRead mode
  readEnd();
  //select card
  SET_LOW(SD_PORT, SD_CS);
  // some cards need extra clocks to go to ready state
  spiRec();
  // send command
  spiSend(cmd | 0x40);
  //send argument
  for (int8_t s = 24; s >= 0; s -= 8) spiSend(arg >> s);
  //send CRC
  spiSend(crc);
  //wait for not busy
  for (uint8_t retry = 0; (r1 = spiRec()) == 0xFF && retry != 0XFF; retry++);
  return r1;
}

void readEnd(void){
  if (inBlock_) {
    // skip data and crc
    SPDR = 0XFF;
    while (offset_++ < 513) {
      while(!(SPSR & (1 << SPIF)));
      SPDR = 0XFF;
    }
    while(!(SPSR & (1 << SPIF)));//wait for last crc byte
    SET_HIGH(SD_PORT, SD_CS);
    inBlock_ = 0;
  }
}

uint8_t sdWaitStartBlock(void){
  uint8_t r;
  uint16_t retry;
  //wait for start of data
  for (retry = 0; ((r = spiRec()) == 0XFF) && retry != 10000; retry++);
  if (r == DATA_START_BLOCK) return 1;
  error(5);
  return 0;
}

uint8_t cdRawRead(uint32_t block, uint16_t offset, uint8_t *dst, uint16_t count)
{
  if (count == 0) return 1;
  if ((count + offset) > 512) {
    error(5);
    return 0;
  }
  if (!inBlock_ || block != block_ || offset < offset_) {
    block_ = block;
    //use address if not SDHC card
    if (type_ != SD_CARD_TYPE_SDHC) block <<= 9;
    if (cardCommand(CMD17, block, 0XFF)) {
      error(4);
      return 0;
    }
    if (!sdWaitStartBlock()) return 0;
    offset_ = 0;
    inBlock_ = 1;
  }
  //start first spi transfer
  SPDR = 0XFF;
  //skip data before offset
  for (;offset_ < offset; offset_++) {
    while(!(SPSR & (1 << SPIF)));
    SPDR = 0XFF;
  }
  //transfer data
  uint16_t n = count - 1;
  for (uint16_t i = 0; i < n; i++) {
    while(!(SPSR & (1 << SPIF)));
    dst[i] = SPDR;
    SPDR = 0XFF;
  }
  while(!(SPSR & (1 << SPIF)));// wait for last byte
  dst[n] = SPDR;
  offset_ += count;
  if (!partialBlockRead_ || offset_ >= 512) readEnd();
  return 1;
}


void error(uint8_t e){
  displayBegin();
  displayClean();
  displayPrintHex(0xef, 0, 0);
  displayPrintHex(0xf0, 1, 0);
  displayPrintHex(e, 2, 0);
  displayUpdate();
}
