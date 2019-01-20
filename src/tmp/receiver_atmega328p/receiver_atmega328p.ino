#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(0,1);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int LED = 5;


void setup(void){
  
  radio.begin();
  radio.openReadingPipe(1,pipe);

  radio.setChannel(5);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_250KBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_LOW);  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  
  radio.startListening();
  //radio.printDetails();
  
  pinMode(LED, OUTPUT);
}

void loop(void){    
    if (msg[0] != 0){

      if (msg[0] == 111){
        digitalWrite(LED, HIGH);
      }
      if(msg[0] == 222){
        digitalWrite(LED, LOW);
      }

    }
    radio.read(msg, 1);
    delay(100);
}
