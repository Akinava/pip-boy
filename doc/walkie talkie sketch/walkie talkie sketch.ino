/* Спец проект youtube - канала VirF Live Productions
Адрес канала www.youtube.com/c/VirFLiveProductions
Группа посвященная каналу https://vk.com/virf_live_productions
version 1.2 от 12.05.17
*/
#include <RF24.h>   
#include <SPI.h>
#include <RF24V.h>
RF24 radio(7,8);    
RF24V Sound(radio,0);
boolean flag = 0;
boolean V_Up = 0;
boolean V_Down = 0;
void setup() {
pinMode(5, INPUT_PULLUP);
pinMode(6, INPUT_PULLUP);
pinMode(15, INPUT_PULLUP);
pinMode(4, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
radio.begin();
radio.setChannel(0x4b);
radio.setAutoAck(0);
radio.setDataRate(RF24_1MBPS);
Sound.begin();
radio.setCRCLength(RF24_CRC_8);
digitalWrite(4,1);}
void loop(){
if (digitalRead(15) == 1 && flag == 0){Sound.reader();
digitalWrite(9,0); digitalWrite(10,0); flag = 1;delay(100);}
if (digitalRead(15) == 0 && flag == 1){Sound.transfer();
flag = 0;delay(100);}
if (digitalRead(5)==0&&V_Down==0){Sound.volume(0);V_Down=1;delay(100);}
if (digitalRead(5)==1&&V_Down==1){V_Down=0;}
if (digitalRead(6)==0&&V_Up==0){Sound.volume(1);V_Up=1;delay(100);}
if (digitalRead(6)==1&&V_Up==1){V_Up=0;}}
/*P.S. данный скетч по мимо своей непосредственной библиотеки "RF24V" 
требует установку основной библиотеки "nRF24L01"
Ссылка на оригинальную библиотеку "nRF24L01" https://yadi.sk/d/RvbUfGFEwjRid
*/

