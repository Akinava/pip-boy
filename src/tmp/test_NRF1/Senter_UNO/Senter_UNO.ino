#include <SPI.h>
#include <NRFLite.h>
#include <OLED_I2C.h>

#define LED_PIN 5
#define SD_CS_PIN 10

void blink();

OLED  myOLED(SDA, SCL, 8);
extern uint8_t TinyFont[];

const static uint8_t RADIO_ID = 2;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 0; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 0;
const static uint8_t PIN_RADIO_CSN = 1;

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

void setup()
{
    myOLED.begin();
    myOLED.setFont(TinyFont);
    
    pinMode(LED_PIN, OUTPUT);
    
    pinMode(SD_CS_PIN, OUTPUT);
    digitalWrite(SD_CS_PIN, HIGH);
    //SPI.transfer(0xAA);
            
    if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
        myOLED.print("Cannot communicate with radio", 0, 0);
        myOLED.update();
        digitalWrite(LED_PIN, HIGH);
        while (1); // Wait here forever.
    }
    
    _radioData.FromRadioId = RADIO_ID;
    //Serial.println("Ready to send");
    digitalWrite(5, LOW);
}

void loop()
{
    _radioData.OnTimeMillis = millis();
    myOLED.print(String(_radioData.OnTimeMillis), 0, 0);
    myOLED.update();

    if (!_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData)))
    {
        _radioData.FailedTxCount++;
    }
    blink();
    delay(1000);
}

void blink() {
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                    // wait
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
}
