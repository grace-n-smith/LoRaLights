void setup() {
    Serial.begin(9600);
    delay(100);
    pinMode(8, OUTPUT);
}

void loop() {
    Serial.println("Starting...");

    digitalWrite(8, HIGH);
    delay(1000);
    digitalWrite(8, LOW);
    delay(1000);
}