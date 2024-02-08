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

void setup() {
  Serial.begin(9600);
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
}

/*
ROOFDECK
PECKER
BMF
BLACK_HOLE
FOO
AIW
BONFIRE
CLAM
LOOPF
DESTINY
TRUNK
*/
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
int OWN_PLACE_VALUE = 512;
int OWN_LED_STATE = 0;
void updateState(int update) {
  FLOOR_STATES = update | (OWN_LED_STATE != 0 ? OWN_PLACE_VALUE : 0);
}

void loop() {
  if(FLOOR_STATES == 0) {
    FLOOR_STATES = 1;
  } else {
    FLOOR_STATES *= 2;
    if(FLOOR_STATES > 1024) FLOOR_STATES = 1;
  }
  writeLights(FLOOR_STATES);
  delay(91);
}
