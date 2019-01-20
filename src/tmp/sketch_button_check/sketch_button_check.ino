#include <OLED_I2C.h>

#define LED 5
#define KEY_A 4
#define KEY_B 2
#define KEY_C 17
#define KEY_U 9
#define KEY_D 6
#define KEY_L 8
#define KEY_R 7 

OLED  myOLED(SDA, SCL, 8);

//extern uint8_t TinyFont[];
extern uint8_t SmallFont[];


void setup()
{
  //pinMode(LED, OUTPUT);
  pinMode(KEY_A, INPUT);
  pinMode(KEY_B, INPUT);
  pinMode(KEY_C, INPUT);
  pinMode(KEY_U, INPUT);
  pinMode(KEY_D, INPUT);
  pinMode(KEY_L, INPUT);
  pinMode(KEY_R, INPUT);
  
  //digitalWrite(LED, LOW);
  digitalWrite(KEY_A, HIGH);
  digitalWrite(KEY_B, HIGH);
  digitalWrite(KEY_C, HIGH);
  digitalWrite(KEY_U, HIGH);
  digitalWrite(KEY_D, HIGH);
  digitalWrite(KEY_L, HIGH);
  digitalWrite(KEY_R, HIGH);
  
  
  myOLED.begin();
  //myOLED.setFont(TinyFont);
  myOLED.setFont(SmallFont);
}

void loop()
{
  myOLED.clrScr();
  myOLED.update();
  //digitalWrite(LED, LOW);
  if(!digitalRead(KEY_A)){
    myOLED.print("BTN A", 0, 3);
    myOLED.update();
    //digitalWrite(LED, HIGH);
    delay(1000);
  }  
  if(!digitalRead(KEY_B)){
    myOLED.print("BTN B", 0, 3);
    myOLED.update();
    delay(1000);
  }
  if(!digitalRead(KEY_C)){
    myOLED.print("BTN C", 0, 3);
    myOLED.update();
    delay(1000);
  }
  if(!digitalRead(KEY_U)){
    myOLED.print("BTN U", 0, 3);
    myOLED.update();
    delay(1000);
  }
  if(!digitalRead(KEY_D)){
    myOLED.print("BTN D", 0, 3);
    myOLED.update();
    delay(1000);
  }
  if(!digitalRead(KEY_L)){
    myOLED.print("BTN L", 0, 3);
    myOLED.update();
    delay(1000);
  }
  if(!digitalRead(KEY_R)){
    myOLED.print("BTN R", 0, 3);
    myOLED.update();
    delay(1000);
  }
}
