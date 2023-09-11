#include <Arduino.h>

// #define DELAY 1000000

boolean exState = LOW;

void impulse();

void setup() {
    DDRB = B00111111;           // Setta come output

    PORTB &= B11000000;         // Resetta tutti
    PORTB |= B00111111;

    Serial.begin(115200);

    pinMode(2, INPUT);
    exState = digitalRead(2);
}

// L'impulso basso deve durare MAX 5us

void loop() 
{
    impulse();
    delayMicroseconds(5000);
}

void impulse()
{
    PORTB &= B11000000;
    PORTB |= B00111111;
}