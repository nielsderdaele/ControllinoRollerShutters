#include "JunkersBoilerProgram.h"
#include <Controllino.h>

JunkersBoilerProgram::JunkersBoilerProgram(byte inputPin) {
    this->lastReadMillis = 0;
    this->inputPin = inputPin;
}

void JunkersBoilerProgram::setup() {
    pinMode(this->inputPin, INPUT);
}

void JunkersBoilerProgram::loop() {
    if ((millis() - this->lastReadMillis) > 2000) {
        this->lastReadMillis = millis();

        int sensorValue = analogRead(this->inputPin);
        
        Serial.print("Boiler input: ");
        Serial.println(sensorValue);
    }
}