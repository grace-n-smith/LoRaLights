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

// initializes the switch state
int buttonPin = 4;         // the number of the input pin
int myPinID = 5;       // the number of the output pin, DEPENDENT ON ID

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
#define TRUNK 5
#define DESTINY 6
#define LOOPF 7
#define CLAM 8
#define BONFIRE 9
#define AIW A7
#define FOO A6
#define BLACKHOLE A5
#define BMF A4
#define PECKER A3
#define ROOFDECK A2

void setup()
{
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to be available
  delay(100);

  Serial.println("Starting...");

  pinMode(LED, OUTPUT);
  // pinMode(RFM95_RST, OUTPUT);
  // digitalWrite(RFM95_RST, HIGH);

  pinMode(TRUNK, OUTPUT);
  pinMode(DESTINY, OUTPUT);
  pinMode(LOOPF, OUTPUT);
  pinMode(CLAM, OUTPUT);
  pinMode(BONFIRE, OUTPUT);
  pinMode(AIW, OUTPUT);
  pinMode(FOO, OUTPUT);
  pinMode(BLACKHOLE, OUTPUT);
  pinMode(BMF, OUTPUT);
  pinMode(PECKER, OUTPUT);
  pinMode(ROOFDECK, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  delay(100);

  // manual reset
  // digitalWrite(RFM95_RST, LOW);
  // delay(10);
  // digitalWrite(RFM95_RST, HIGH);
  // delay(10);
 
  // while (!rf95.init()) {
  //   Serial.println("RF95 failed to init");
  //   while (1);
  // }

  // if (!rf95.setFrequency(RF95_FREQ)) {
  //   Serial.println("setFrequency failed");
  //   while (1);
  // }
 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  // rf95.setTxPower(23, false);
  Serial.println("Blinking");

  digitalWrite(TRUNK, HIGH);
  digitalWrite(DESTINY, HIGH);
  digitalWrite(LOOPF, HIGH);
  digitalWrite(CLAM, HIGH);
  digitalWrite(BONFIRE, HIGH);
  digitalWrite(AIW, HIGH);
  digitalWrite(FOO, HIGH);
  digitalWrite(BLACKHOLE, HIGH);
  digitalWrite(BMF, HIGH);
  digitalWrite(PECKER, HIGH);
  digitalWrite(ROOFDECK, HIGH);
  delay(1000);
  // digitalWrite(TRUNK, LOW);
  // digitalWrite(DESTINY, LOW);
  // digitalWrite(LOOPF, LOW);
  // digitalWrite(CLAM, LOW);
  // digitalWrite(BONFIRE, LOW);
  // digitalWrite(AIW, LOW);
  // digitalWrite(FOO, LOW);
  // digitalWrite(BLACKHOLE, LOW);
  // digitalWrite(BMF, LOW);
  // digitalWrite(PECKER, LOW);
  // digitalWrite(ROOFDECK, LOW);
  // delay(1000);

  Serial.println("Blunk");
}
 
void loop()
{
  digitalWrite(TRUNK, HIGH);
  digitalWrite(DESTINY, HIGH);
  digitalWrite(LOOPF, HIGH);
  digitalWrite(CLAM, HIGH);
  digitalWrite(BONFIRE, HIGH);
  digitalWrite(AIW, HIGH);
  digitalWrite(FOO, HIGH);
  digitalWrite(BLACKHOLE, HIGH);
  digitalWrite(BMF, HIGH);
  digitalWrite(PECKER, HIGH);
  digitalWrite(ROOFDECK, HIGH);

  return;
  
  reading = digitalRead(buttonPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce)
  {
    if (state == HIGH) {
      state = LOW;
      Serial.println("Toggled state off");
    } else {
      state = HIGH;
      Serial.println("Toggled state on");
    }

    time = millis();
  }
  digitalWrite(myPinID, state);
  previous = reading;
  return;

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
        digitalWrite(TRUNK, HIGH);
      } else {
        digitalWrite(TRUNK, LOW);
      }
      if (switchArray[1] == '0') {
        digitalWrite(DESTINY, HIGH);
      } else {
        digitalWrite(DESTINY, LOW);
      }
      if (switchArray[2] == '0') {
        digitalWrite(LOOPF, HIGH);
      } else {
        digitalWrite(LOOPF, LOW);
      }
      if (switchArray[3] == '0') {
        digitalWrite(CLAM, HIGH);
      } else {
        digitalWrite(CLAM, LOW);
      }
      if (switchArray[4] == '0') {
        digitalWrite(BONFIRE, HIGH);
      } else {
        digitalWrite(BONFIRE, LOW);
      }
      if (switchArray[5] == '0') {
        digitalWrite(AIW, HIGH);
      } else {
        digitalWrite(AIW, LOW);
      }
      if (switchArray[6] == '0') {
        digitalWrite(FOO, HIGH);
      } else {
        digitalWrite(FOO, LOW);
      }
      if (switchArray[7] == '0') {
        digitalWrite(BLACKHOLE, HIGH);
      } else {
        digitalWrite(BLACKHOLE, LOW);
      }
      if (switchArray[8] == '0') {
        digitalWrite(BMF, HIGH);
      } else {
        digitalWrite(BMF, LOW);
      }
      if (switchArray[9] == '0') {
        digitalWrite(PECKER, HIGH);
      } else {
        digitalWrite(PECKER, LOW);
      }
      if (switchArray[10] == '0') {
        digitalWrite(ROOFDECK, HIGH);
      } else {
        digitalWrite(ROOFDECK, LOW);
      }
      
    } else {
      //Serial.println("Receive failed");
    }

    // Sending back
    uint8_t reply = reading == HIGH ? 1 : 0;
    rf95.send(&reply, 1);
    rf95.waitPacketSent();
  }
}
