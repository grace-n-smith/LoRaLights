// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
 
#include <SPI.h>
#include <RH_RF95.h>

// for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3


#if defined(ESP8266)
  /* for ESP w/featherwing */ 
  #define RFM95_CS  2    // "E"
  #define RFM95_RST 16   // "D"
  #define RFM95_INT 15   // "B"
 
#elif defined(ESP32)  
  /* ESP32 feather w/wing */
  #define RFM95_RST     27   // "A"
  #define RFM95_CS      33   // "B"
  #define RFM95_INT     12   //  next to A
 
#elif defined(NRF52)  
  /* nRF52832 feather w/wing */
  #define RFM95_RST     7   // "A"
  #define RFM95_CS      11   // "B"
  #define RFM95_INT     31   // "C"
  
#elif defined(TEENSYDUINO)
  /* Teensy 3.x w/wing */
  #define RFM95_RST     9   // "A"
  #define RFM95_CS      10   // "B"
  #define RFM95_INT     4    // "C"
#endif
 
// Identify Local Unit
#define UNIT_ID 0
// can be anywhere from 0-10

// initializes the switch state
int inPin = 1;         // the number of the input pin
int outPin = 13;       // the number of the output pin, DEPENDENT ON ID

int state = LOW;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = HIGH;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long time = 0;           // the last time the output pin was toggled
unsigned long debounce = 200UL;   // the debounce time, increase if the output flickers

 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 13

//define char (but kinda bool) array for switch
#define NUM_UNITS 11
char switchArray[NUM_UNITS] = "";

//switches
uint8_t SWITCH0 = 4;
uint8_t SWITCH1 = 5;
uint8_t SWITCH2 = 6;
uint8_t SWITCH3 = 7;
uint8_t SWITCH4 = 8;
uint8_t SWITCH5 = 9;
uint8_t SWITCH6 = 5;
uint8_t SWITCH7 = 6;
uint8_t SWITCH8 = 9;
uint8_t SWITCH9 = 10;
uint8_t SWITCH10 = 11;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  pinMode(SWITCH0, OUTPUT);
  pinMode(SWITCH1, OUTPUT);
  pinMode(SWITCH2, OUTPUT);
  pinMode(SWITCH3, OUTPUT);
  pinMode(SWITCH4, OUTPUT);
  pinMode(SWITCH5, OUTPUT);
  pinMode(SWITCH6, OUTPUT);
  pinMode(SWITCH7, OUTPUT);
  pinMode(SWITCH8, OUTPUT);
  pinMode(SWITCH9, OUTPUT);
  pinMode(SWITCH10, OUTPUT);
  pinMode(inPin,  INPUT);
  pinMode(outPin, OUTPUT);
 
  //Serial.begin(115200);
  //while (!Serial) {
  //  delay(1);
  //}
  delay(100);
 
  //Serial.println("Feather LoRa RX Test!");
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    //Serial.println("LoRa radio init failed");
    //Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  //Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    //Serial.println("setFrequency failed");
    while (1);
  }
  //Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  digitalWrite(SWITCH0, HIGH);
  digitalWrite(SWITCH1, HIGH);
  digitalWrite(SWITCH2, HIGH);
  digitalWrite(SWITCH3, HIGH);
  digitalWrite(SWITCH4, HIGH);
  digitalWrite(SWITCH5, HIGH);
  digitalWrite(SWITCH6, HIGH);
  digitalWrite(SWITCH7, HIGH);
  digitalWrite(SWITCH8, HIGH);
  digitalWrite(SWITCH9, HIGH);
  digitalWrite(SWITCH10, HIGH);
  delay(1000);
  digitalWrite(SWITCH0, LOW);
  digitalWrite(SWITCH1, LOW);
  digitalWrite(SWITCH2, LOW);
  digitalWrite(SWITCH3, LOW);
  digitalWrite(SWITCH4, LOW);
  digitalWrite(SWITCH5, LOW);
  digitalWrite(SWITCH6, LOW);
  digitalWrite(SWITCH7, LOW);
  digitalWrite(SWITCH8, LOW);
  digitalWrite(SWITCH9, LOW);
  digitalWrite(SWITCH10, LOW);
  delay(1000);
}
 
void loop()
{

    reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce)
  {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();
  }

  digitalWrite(outPin, state);

  previous = reading;
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
 
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      //RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.print("Got: ");
      //Serial.println((char*)buf);
      //Serial.print("RSSI: ");
      //Serial.println(rf95.lastRssi(), DEC);
      strcpy(switchArray, "");

      switchArray[0] = (char)buf[0];
      switchArray[1] = (char)buf[1];
      switchArray[2] = (char)buf[2];
      switchArray[3] = (char)buf[3];
      switchArray[4] = (char)buf[4];
      switchArray[5] = (char)buf[5];
      switchArray[6] = (char)buf[6];
      switchArray[7] = (char)buf[7];
      switchArray[8] = (char)buf[8];
      switchArray[9] = (char)buf[9];
      switchArray[10] = (char)buf[10];

      //sprintf(switchArray, "%s" , (char*)buf);

      if (switchArray[0] == '0') {
        digitalWrite(SWITCH0, HIGH);
      } else {
        digitalWrite(SWITCH0, LOW);
      }
      if (switchArray[1] == '0') {
        digitalWrite(SWITCH1, HIGH);
      } else {
        digitalWrite(SWITCH1, LOW);
      }
      if (switchArray[2] == '0') {
        digitalWrite(SWITCH2, HIGH);
      } else {
        digitalWrite(SWITCH2, LOW);
      }
      if (switchArray[3] == '0') {
        digitalWrite(SWITCH3, HIGH);
      } else {
        digitalWrite(SWITCH3, LOW);
      }
      if (switchArray[4] == '0') {
        digitalWrite(SWITCH4, HIGH);
      } else {
        digitalWrite(SWITCH4, LOW);
      }
      if (switchArray[5] == '0') {
        digitalWrite(SWITCH5, HIGH);
      } else {
        digitalWrite(SWITCH5, LOW);
      }
      if (switchArray[6] == '0') {
        digitalWrite(SWITCH6, HIGH);
      } else {
        digitalWrite(SWITCH6, LOW);
      }
      if (switchArray[7] == '0') {
        digitalWrite(SWITCH7, HIGH);
      } else {
        digitalWrite(SWITCH7, LOW);
      }
      if (switchArray[8] == '0') {
        digitalWrite(SWITCH8, HIGH);
      } else {
        digitalWrite(SWITCH8, LOW);
      }
      if (switchArray[9] == '0') {
        digitalWrite(SWITCH9, HIGH);
      } else {
        digitalWrite(SWITCH9, LOW);
      }
      if (switchArray[10] == '0') {
        digitalWrite(SWITCH10, HIGH);
      } else {
        digitalWrite(SWITCH10, LOW);
      }
      
    }
    else
    {
      //Serial.println("Receive failed");
    }


    // Sending back
    int reply = UNIT_ID
    if (reading == HIGH) {
        reply = reply + 128
    }
    //Serial.println("Sending...");
    delay(10);
    rf95.send(reply);
 
    //Serial.println("Waiting for packet to complete..."); 
    delay(10);
    rf95.waitPacketSent();


  }
}
