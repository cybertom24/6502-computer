#include <Arduino.h>

#define PIN 2
#define DELAY 5000

void setup() {
  // put your setup code here, to run once:

    pinMode(PIN, OUTPUT);
}

void loop() {
    digitalWrite(PIN, LOW);
    delayMicroseconds(DELAY);
    digitalWrite(PIN, HIGH);
    delayMicroseconds(DELAY);
}