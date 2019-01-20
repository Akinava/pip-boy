#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(12,11);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void){
  Serial.begin(9600);
  Serial.println(F("start"));
  
  radio.begin();
  radio.openReadingPipe(1,pipe);

  radio.setChannel(5);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_250KBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_LOW);  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  
  radio.startListening();
  radio.printDetails();
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void){    
    if (msg[0] != 0){
      Serial.println(F("recived new data"));
      Serial.println(msg[0]);

      if (msg[0] == 111){
        digitalWrite(LED_BUILTIN, HIGH);
      }
      if(msg[0] == 222){
        digitalWrite(LED_BUILTIN, LOW);
      }

      if(!radio.available()){
        Serial.println("No radio available");
      }
    }
    radio.read(msg, 1);
    delay(100);
}
