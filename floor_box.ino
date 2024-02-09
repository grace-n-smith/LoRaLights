// Arduino9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Arduino9x_TX

#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 2
#define RFM95_INT 3

#define BUTTON 4

const int TRUNK = A5;
const int DESTINY = A4;
const int LOOPF = A3;
const int CLAM = A2;
const int BONFIRE = A1; 
const int ROOFDECK = A0;

const int AIW = 5;
const int FOO = 6;
const int BLACK_HOLE = 7;
const int BMF = 8;
const int PECKER = 9;

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void writeLights(int lights) {
  digitalWrite(ROOFDECK, (lights & 1024) != 0 ? HIGH : LOW);
  digitalWrite(PECKER, (lights & 512) != 0 ? HIGH : LOW);
  digitalWrite(BMF, (lights & 256) != 0 ? HIGH : LOW);
  digitalWrite(BLACK_HOLE, (lights & 128) != 0 ? HIGH : LOW);
  digitalWrite(FOO, (lights & 64) != 0 ? HIGH : LOW);
  digitalWrite(AIW, (lights & 32) != 0 ? HIGH : LOW);
  digitalWrite(BONFIRE, (lights & 16) != 0 ? HIGH : LOW);
  digitalWrite(CLAM, (lights & 8) != 0 ? HIGH : LOW);
  digitalWrite(LOOPF, (lights & 4) != 0 ? HIGH : LOW);
  digitalWrite(DESTINY, (lights & 2) != 0 ? HIGH : LOW);
  digitalWrite(TRUNK, (lights & 1) != 0 ? HIGH : LOW);
}


int lastReading = LOW;



bool buttonPressed() {
  int reading = digitalRead(BUTTON);
  bool pressed = lastReading == LOW && reading == HIGH;
  lastReading = reading;
  return pressed;
}




int FLOOR_STATES = 0;
int OWN_PLACE_VALUE = 1 << 1;
uint8_t OWN_LED_STATE = 0;




//void updateState(int update) {
//  Serial.println("old:");
//  Serial.println(FLOOR_STATES);
//  FLOOR_STATES = update ^ (OWN_LED_STATE != 0 ? OWN_PLACE_VALUE : 0);
//  Serial.println("new:");
//  Serial.println(FLOOR_STATES);
//}



void setup() 
{     
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(TRUNK, OUTPUT);
  pinMode(DESTINY, OUTPUT);
  pinMode(LOOPF, OUTPUT);
  pinMode(CLAM, OUTPUT);
  pinMode(BONFIRE, OUTPUT);
  pinMode(AIW, OUTPUT);
  pinMode(FOO, OUTPUT);
  pinMode(BLACK_HOLE, OUTPUT);
  pinMode(BMF, OUTPUT);
  pinMode(PECKER, OUTPUT);
  pinMode(ROOFDECK, OUTPUT);

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //while (!Serial);
  //Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa RX Test!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
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
}

void loop()
{
  // toggle state if button press detected
  if (buttonPressed()) {
    FLOOR_STATES ^= OWN_PLACE_VALUE;
  }

  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      int recvId = (buf[0] << 8) + buf[1];
      int recvStates = (buf[2] << 8) + buf[3];

      RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.print("Got ID: ");
      //Serial.println(recvId);
      //Serial.print("Got states: ");
      //Serial.println(recvStates);
      //Serial.print("RSSI: ");
      //Serial.println(rf95.lastRssi(), DEC);

      if (recvId == OWN_PLACE_VALUE) {
        // FLOOR_STATES |= recvStates;
        FLOOR_STATES = (FLOOR_STATES & OWN_PLACE_VALUE) | (recvStates & ~OWN_PLACE_VALUE);
        // Send a reply
        uint8_t data[] = { (OWN_PLACE_VALUE & 0xFF00) >> 8, OWN_PLACE_VALUE & 0xFF, (FLOOR_STATES & 0xFF00) >> 8, FLOOR_STATES & 0xFF};
        rf95.send(data, sizeof(data));
        rf95.waitPacketSent();
        //Serial.println("Sent a reply");
      }
    }
    else
    {
      //Serial.println("Receive failed");
    }
  }
  writeLights(FLOOR_STATES);
}